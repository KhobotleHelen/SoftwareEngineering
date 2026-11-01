# Usability test sheet (NFR-07: "clear menus, checked with one test user per role")
Run with ONE person per role who has not seen the system. Give no help except the printed task. Time each task and tick
whether they finished without help. Use the demo database (`lms_seed demo.db 50`, password Password123).

| Role / user | Task | Finished without help? (Y/N) | Time (min) | Confusion or error message that puzzled them | Suggested fix |
|---|---|---|---|---|---|
| Student (student1) | Find the assignment, submit a file, later see the released mark | | | | |
| Lecturer (lecturer2) | Create an assignment, mark a submission, give a class extension | | | | |
| HoD (hod_sci) | Review a mark, approve it, release it | | | | |
| Administrator (admin) | Create a student, enrol them in a course | | | | |

Pass rule suggested for NFR-07: every test user completes their task without help in under 10 minutes.
Write the results (and any fix you make) in Section 7.3 and update the risk register (risk 4).
