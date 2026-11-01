# CR1: class-wide extensions and extension display (release v1.1)

**Request (school):** "A lecturer should be able to extend the deadline for a whole class, not just an individual
student, and extensions should be shown on the student's assignment list."

## Classification
Perfective maintenance: nothing is faulty and the environment is unchanged; users want a new capability
(whole-class extension) and a better display (extension shown on the list).

## Impact analysis (predicted) and what actually changed
| Area | Predicted | Actual change |
|---|---|---|
| Requirements | new FR-15 class extension; FR-06 changed to show extensions | FR-15 added (Should); FR-06 wording extended |
| Use cases | "Grant extension" gets an alternative flow "whole class" | same |
| Classes | `AssignmentService` (new method), `StudentAssignmentView` (new fields), `ConsoleApp` (lecturer menu, student list) | `AssignmentService.h/.cpp` (`grantClassExtension`, `ClassExtensionResult`, `DeadlineResolver` dependency), `StudentAssignmentView` (`effectiveDeadline`, `extended`), `AppContext.h` (wiring), `ConsoleApp.cpp` |
| Tables | none | none: one `Extension` row per enrolled student, written in one transaction |
| Late-penalty / submission code | unchanged (reads deadline via `DeadlineResolver`) | unchanged (confirmed by regression run) |
| Tests | extend `Extensions.*`, `SystemTest.ExtensionFlowThroughUi`; add new | one script line updated (the menu now asks "one student or whole class"); 10 new tests |
| Build | version | `CMakeLists.txt` 1.0.0 -> 1.1.0; banner shows the version |

## Design decisions
- A class extension **never shortens** a later extension already given to an individual student (those students are
  counted as "kept").
- Everything happens in **one transaction**: all enrolled students are extended or nobody is.
- A reason is required and is stored as `Class extension: <reason>` so it can be told apart from individual ones.
- The student list shows `due <original> | EXTENDED to <effective>`, using `DeadlineResolver`, the same rule used by
  submission and marking, so the list can never disagree with the penalty calculation.

## Results
- Regression: all 112 tests of v1.0 pass unchanged except one script line (`docs/qa/regression_v1.1.txt`).
- New: 10 tests (`tests/test_cr1_class_extension.cpp`), all pass. Total 122.
- Business-logic line coverage 98.4% (745 of 757); cppcheck 0 findings; CI green.
