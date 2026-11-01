// CR1 (release v1.1): "A lecturer should be able to extend the deadline for a whole class, not just an
// individual student, and extensions should be shown on the student's assignment list."
#include <sstream>

#include "TestEnv.h"
#include "ui/ConsoleApp.h"

using namespace lms;
using lms::testing::Env;

namespace {

std::string runUi(Env& e, const std::string& script) {
    std::istringstream in(script);
    std::ostringstream out;
    ConsoleApp app(e.ctx, in, out, e.clock);
    app.run();
    return out.str();
}

bool has(const std::string& hay, const std::string& needle) {
    return hay.find(needle) != std::string::npos;
}

}  // namespace

TEST(ClassExtension, EveryEnrolledStudentGetsTheNewDeadlineAndOthersDoNot) {
    Env e;
    const DateTime until = DateTime::parse("2026-11-27 12:00");
    const ClassExtensionResult r =
        e.ctx.assignments.grantClassExtension(*e.lecturer, e.assignmentId, until, "Power cut");
    EXPECT_EQ(r.extended, 2);  // s1 and s2 are enrolled
    EXPECT_EQ(r.keptLater, 0);
    EXPECT_EQ(e.ctx.submissions.effectiveDeadline(*e.s1, e.assignmentId), until);
    EXPECT_EQ(e.ctx.submissions.effectiveDeadline(*e.s2, e.assignmentId), until);
    EXPECT_FALSE(e.ctx.extensions.find(e.assignmentId, e.outsider->id()));  // not enrolled: untouched
    const auto stored = e.ctx.extensions.find(e.assignmentId, e.s1->id());
    ASSERT_TRUE(stored);
    EXPECT_EQ(stored->grantedBy, e.lecturer->id());
    EXPECT_EQ(stored->reason, "Class extension: Power cut");
}

TEST(ClassExtension, NeverShortensALaterIndividualExtension) {
    Env e;
    e.ctx.assignments.grantExtension(*e.lecturer, e.assignmentId, e.s1->id(),
                                     DateTime::parse("2026-12-05 12:00"), "Hospital");
    const ClassExtensionResult r = e.ctx.assignments.grantClassExtension(
        *e.lecturer, e.assignmentId, DateTime::parse("2026-11-27 12:00"), "Power cut");
    EXPECT_EQ(r.extended, 1);
    EXPECT_EQ(r.keptLater, 1);
    EXPECT_EQ(e.ctx.submissions.effectiveDeadline(*e.s1, e.assignmentId).toString(), "2026-12-05 12:00:00");
    EXPECT_EQ(e.ctx.submissions.effectiveDeadline(*e.s2, e.assignmentId).toString(), "2026-11-27 12:00:00");
}

TEST(ClassExtension, LaterClassDateReplacesAnEarlierIndividualOne) {
    Env e;
    e.ctx.assignments.grantExtension(*e.lecturer, e.assignmentId, e.s1->id(),
                                     DateTime::parse("2026-11-22 12:00"), "Ill");
    const auto r = e.ctx.assignments.grantClassExtension(*e.lecturer, e.assignmentId,
                                                         DateTime::parse("2026-11-27 12:00"), "Power cut");
    EXPECT_EQ(r.extended, 2);
    EXPECT_EQ(e.ctx.submissions.effectiveDeadline(*e.s1, e.assignmentId).toString(), "2026-11-27 12:00:00");
}

TEST(ClassExtension, InvalidRequestsAreRejectedAndNothingIsSaved) {
    Env e;
    const DateTime later = DateTime::parse("2026-11-27 12:00");
    auto grant = [&](const User& u, Id a, const DateTime& d, const char* reason) {
        return e.ctx.assignments.grantClassExtension(u, a, d, reason);
    };
    EXPECT_THROW(grant(*e.lecturer, e.assignmentId, e.deadline(), "r"), ValidationError);  // not later
    EXPECT_THROW(grant(*e.lecturer, e.assignmentId, e.deadline().plusSeconds(-1), "r"), ValidationError);
    EXPECT_THROW(grant(*e.lecturer, e.assignmentId, later, "   "), ValidationError);  // reason needed
    EXPECT_THROW(grant(*e.lecturer, 99999, later, "r"), NotFoundError);
    EXPECT_THROW(grant(*e.otherLecturer, e.assignmentId, later, "r"),
                 AuthorizationError);  // not their course
    EXPECT_THROW(grant(*e.hod, e.assignmentId, later, "r"), AuthorizationError);
    EXPECT_THROW(grant(*e.s1, e.assignmentId, later, "r"), AuthorizationError);
    EXPECT_FALSE(e.ctx.extensions.find(e.assignmentId, e.s1->id()));
    EXPECT_NO_THROW(
        grant(*e.lecturer, e.assignmentId, e.deadline().plusSeconds(1), "boundary"));  // just later
}

TEST(ClassExtension, CourseWithNoStudentsGivesAClearError) {
    Env e;
    const Id emptyCourse = e.ctx.academic.createCourse(*e.admin, e.deptId, "Empty course");
    e.ctx.academic.assignLecturer(*e.admin, emptyCourse, e.lecturer->id());
    const Id a = e.ctx.assignments.createAssignment(*e.lecturer, emptyCourse, "T", "d",
                                                    DateTime::parse("2026-12-01 08:00"), 10);
    EXPECT_THROW(
        e.ctx.assignments.grantClassExtension(*e.lecturer, a, DateTime::parse("2026-12-05 08:00"), "r"),
        BusinessRuleError);
}

TEST(ClassExtension, IsAllOrNothingWhenADatabaseWriteFails) {
    Env e;
    struct FailingOnSecond : IExtensionRepository {
        IExtensionRepository& real;
        int calls = 0;
        explicit FailingOnSecond(IExtensionRepository& r) : real(r) {}
        void upsert(const Extension& x) override {
            if (++calls == 2) throw DatabaseError("simulated failure");
            real.upsert(x);
        }
        std::optional<Extension> find(Id a, Id s) override { return real.find(a, s); }
    } failing(e.ctx.extensions);
    AssignmentService svc(e.ctx.assignmentsRepo, failing, e.ctx.submissionsRepo, e.ctx.enrolments,
                          e.ctx.users, e.ctx.deadlines, e.db, e.ctx.access, e.clock);
    EXPECT_THROW(
        svc.grantClassExtension(*e.lecturer, e.assignmentId, DateTime::parse("2026-11-27 12:00"), "r"),
        DatabaseError);
    EXPECT_FALSE(e.ctx.extensions.find(e.assignmentId, e.s1->id()));  // first student's row rolled back
    EXPECT_FALSE(e.ctx.extensions.find(e.assignmentId, e.s2->id()));
}

TEST(ClassExtension, LateWorkBecomesOnTimeAndIsNotPenalised) {  // regression link to FR-10
    Env e;
    e.ctx.assignments.grantClassExtension(*e.lecturer, e.assignmentId, e.deadline().plusDays(3), "Power cut");
    const Id sid = e.submitAt(e.deadline().plusDays(2), *e.s2);
    EXPECT_EQ(e.ctx.submissionsRepo.findById(sid)->status, SubmissionStatus::Submitted);
    EXPECT_DOUBLE_EQ(e.ctx.assessment.recordMark(*e.lecturer, sid, 88, "Good", "").finalMark, 88.0);
}

TEST(ExtensionDisplay, AssignmentListShowsEffectiveDeadlineAndExtendedFlag) {
    Env e;
    e.ctx.assignments.grantExtension(*e.lecturer, e.assignmentId, e.s1->id(),
                                     DateTime::parse("2026-11-25 12:00"), "Ill");
    const auto v1 = e.ctx.assignments.listForStudent(*e.s1);
    const auto v2 = e.ctx.assignments.listForStudent(*e.s2);
    ASSERT_EQ(v1.size(), 1u);
    EXPECT_TRUE(v1[0].extended);
    EXPECT_EQ(v1[0].effectiveDeadline.toString(), "2026-11-25 12:00:00");
    EXPECT_EQ(v1[0].assignment.deadline, e.deadline());  // original deadline still available
    EXPECT_FALSE(v2[0].extended);
    EXPECT_EQ(v2[0].effectiveDeadline, e.deadline());
}

TEST(ClassExtensionUi, LecturerExtendsWholeClassAndStudentsSeeItOnTheirList) {
    Env e;
    std::string out = runUi(e, "lect1\nPassword123\n4\n1\n2\n2026-11-27 12:00\nPower cut\n0\n\n");
    EXPECT_TRUE(has(out, "Class extension recorded: 2 student(s) extended"));
    out = runUi(e, "stud2\nPassword123\n2\n0\n\n");
    EXPECT_TRUE(has(out, "due 2026-11-20 23:59:00 | EXTENDED to 2026-11-27 12:00:00"));
    out = runUi(e, "outsider\nPassword123\n2\n0\n\n");
    EXPECT_FALSE(has(out, "EXTENDED"));
}

TEST(ClassExtensionUi, ShowsWhenALaterIndividualExtensionWasKeptAndRejectsBadChoices) {
    Env e;
    e.ctx.assignments.grantExtension(*e.lecturer, e.assignmentId, e.s1->id(),
                                     DateTime::parse("2026-12-05 12:00"), "Ill");
    std::string out = runUi(e, "lect1\nPassword123\n4\n1\n2\n2026-11-27 12:00\nPower cut\n0\n\n");
    EXPECT_TRUE(has(out, "1 student(s) extended, 1 already had a later extension (kept)"));
    out = runUi(e, "lect1\nPassword123\n4\n1\n9\n0\n\n");
    EXPECT_TRUE(has(out, "Please enter 1 or 2"));
    out = runUi(e, "lect1\nPassword123\n4\n1\n2\n2026-11-10 12:00\nr\n0\n\n");  // earlier than the original
    EXPECT_TRUE(has(out, "must be later than the original deadline"));
}
