# Release notes

## v1.1 (CR1)
- NEW: a lecturer can extend the deadline for the whole class of an assignment (menu 4, option 2). Students who already
  have a later extension keep it. All-or-nothing.
- CHANGED: the student's assignment list shows `EXTENDED to <date>` when an extension applies.
- Tests: 122 (112 regression + 10 new), all passing. No database change: v1.0 databases work unchanged.

## v1.0
First release: all Must requirements FR-01..FR-13 and NFR-03..06, NFR-08. Known limitations: file path stored (not a copy);
no lock-out or session time-out; database not encrypted; approved marks cannot be re-opened.
