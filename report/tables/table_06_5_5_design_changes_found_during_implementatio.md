# Table 6: Design changes found during implementation

From the final report, section "5.5 Design changes found during implementation".

| Change | Reason |
| --- | --- |
| `head_id` column on `Department` (nullable foreign key to `User`) | The design required HoDs to be checked against their department but had no way to record which department an HoD heads |
| Two triggers on `Submission` | A mark must never exceed the assignment maximum; checked in the application and again in the database |
| Seven indexes | To meet the 2-second targets with 800 students |
| `UserFactory` class (third design pattern) | One place that builds the right `User` subclass from a stored role |
| Component services realised as `AuthService`, `AccessControl`, `UserService`, `AcademicService`, `AssignmentService`, `SubmissionService`, `AssessmentService` | Direct realisation of the component diagram |
| Helpers `DeadlineResolver`, `IClock`, `IPasswordHasher`, `ITransactionRunner`, `AppContext` | Single place for the effective deadline; testable time, hashing and transactions; one composition root |
| Penalty computed when the lecturer marks; raw mark and penalty kept in the audit reason | Keeps the schema unchanged and makes every audit row explain itself |
