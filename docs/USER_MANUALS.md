# User manuals (v1.1)

All outputs below were captured from the running v1.1 program (the passwords typed at each prompt are hidden by the program). The demonstration school has Sciences (Mathematics, Physics) and Humanities departments; accounts are `admin`, `lecturer1` to `lecturer5`, `hod_sci`, `hod_hum` and `student1` to `student5`.

## Student manual

1. Start the program and type your username and password.
2. Option 2 (My assignments) shows each assignment, its deadline, any extension (`EXTENDED to ...`), the maximum mark and your submission status.
3. Option 3 (Submit or resubmit work): type the assignment number, the path of your file, and `y` to confirm. You may resubmit as many times as you like before the deadline; the latest version is marked. Work after the deadline is accepted, but a late penalty applies (10% of the maximum mark for each day late, for up to three days; after that the mark is zero).
4. Option 4 (My released marks and feedback) shows a mark only after the Head of Department has released it; until then it says `not released yet`.
5. Option 5 changes your password. Option 0 logs out.

```text
Moderated Learning Management System v1.1.0
Username: student1
Password: 
Welcome, student1 (Student).
Choose an option: 2
  [1] Essay 1 | due 2026-11-10 23:59:00 | EXTENDED to 2026-11-13 23:59:00 | max 100.00 | not submitted
Choose an option: 3
  [1] Essay 1 | due 2026-11-10 23:59:00 | EXTENDED to 2026-11-13 23:59:00 | max 100.00 | not submitted
Assignment number to submit (or resubmit): 1
Your deadline for this assignment: 2026-11-13 23:59:00
Path of the file to submit: essay1.pdf
Warning: that file was not found on this computer.
Submit this path anyway? (y/n): y
Confirm submission (y/n): y
Submission received on time.
Choose an option: 4
  [1] Physics - Essay 1: not released yet
Choose an option: 0
You have been logged out.
```

After release, the student sees the mark and the written feedback:

```text
Moderated Learning Management System v1.1.0
Username: student1
Password: 
Welcome, student1 (Student).
Choose an option: 4
  [1] Physics - Essay 1: 85.00 / 100.00
      Feedback: Clear argument, add more examples.
Choose an option: 0
You have been logged out.
```

## Lecturer manual

1. Option 2 (Create an assignment): choose the course number, then type the title, instructions, deadline and maximum mark.
2. Option 4 (Grant an extension): type the assignment number, then 1 for one student (and the student's user number) or 2 for the whole class, then the new deadline and the reason. A class extension never shortens a later extension that a student already has.
3. Option 5 (View submissions) lists submissions with status and days late.
4. Option 6 (Mark a submission): type the submission number, the mark before any late penalty, written feedback (required) and, when you change an existing mark, the reason (required). The system applies the late penalty and shows the result. The student cannot see the mark yet.
5. Option 7 (View mark history) shows who changed a mark, when, from what to what, and why. If the Head of Department returns a mark for correction, it appears in option 5 with the HoD's comment; mark again with a reason.

```text
Moderated Learning Management System v1.1.0
Username: lecturer2
Password: 
Welcome, lecturer2 (Lecturer).
Choose an option: 5
Assignment number: 1
  [1] student1 | Submitted | submitted 2026-10-31 19:24:39 | late days 0 | file essay1.pdf
Choose an option: 6
Submission number: 1
Mark (before any late penalty): 85
Written feedback: Clear argument, add more examples.
Reason (required when changing an existing mark; optional for a first mark): 
Mark saved. Entered 85.00, final mark 85.00.
It will be visible to the student only after the Head of Department approves and releases it.
Choose an option: 7
Submission number: 1
  2026-10-31 19:24:44 | lecturer2 | - -> 85.00 | Initial mark [raw mark 85.00/100.00]
Choose an option: 0
You have been logged out.
```

## Head of Department manual

1. Option 1 (Marks waiting) lists the marks of your department: `AWAITING REVIEW` (marked, not yet checked) and `APPROVED, awaiting release`.
2. Option 2 (Review a submission): type the submission number; the mark, feedback and history are shown. Type `a` to approve, or `c` and the reason to return it to the lecturer.
3. Option 3 (Release approved marks): type a submission number to release one mark, or `c` and the course number to release all approved marks of that course. Only released marks become visible to students.
4. Option 4 shows the mark history. Option 0 logs out.

```text
Moderated Learning Management System v1.1.0
Username: hod_sci
Password: 
Welcome, hod_sci (HoD).
Choose an option: 1
  [1] Physics | Essay 1 | student1 | mark 85.00/100.00 | AWAITING REVIEW
Choose an option: 2
  [1] Physics | Essay 1 | student1 | mark 85.00/100.00 | AWAITING REVIEW
Submission number to review: 1
Student: student1
Mark: 85.00 / 100.00
Feedback: Clear argument, add more examples.
  history: 2026-10-31 19:24:44 lecturer2 - -> 85.00 (Initial mark [raw mark 85.00/100.00])
Decision: (a)pprove, (c)orrection required, anything else cancels: a
Comments (optional): Fine
Approved. Use 'Release approved marks' to make it visible to the student.
Choose an option: 3
  [1] Mathematics
  [2] Physics
Enter a submission number to release one mark, or 'c' to release all approved marks of a course: 1
Mark released to the student.
Choose an option: 0
You have been logged out.
```

## Administrator manual

1. First start: the program asks for the administrator username and password (twice).
2. Option 1 creates an account (username, role Student, Lecturer, HoD or Administrator, initial password). Option 3 activates or deactivates an account; option 4 resets a password.
3. Option 5 creates a department; option 6 sets the Head of Department (department number, then the HoD's user number); option 7 creates a course; option 8 assigns a lecturer to a course; option 9 enrols a student in a course; option 10 lists departments and courses.
4. Set up in this order: departments, HoD accounts and heads, courses, lecturer accounts and assignments, student accounts and enrolments.

