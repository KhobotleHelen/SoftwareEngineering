# Git branching strategy (Phase 6)
`main` always builds and passes CI. Work happens on short-lived branches (`feature/<topic>`, `fix/<topic>`), merged to
`main` by pull request after CI is green and the self-review checklist is ticked. Commit messages: imperative, one
logical change. Releases are annotated tags (`v1.0`, `v1.1`).

## How to deliver v1.1 (CR1) with a real branch + pull request (do this once, it is the evidence for Phase 6)
```
git checkout main && git pull
git checkout -b feature/cr1-class-extension
# copy the v1.1 files over your working copy (or unzip lms_v1.1_source.zip over it), then commit in small steps:
git add src/services/AssignmentService.h src/services/AssignmentService.cpp src/services/AppContext.h
git commit -m "CR1: add class-wide extension to AssignmentService"
git add src/ui/ConsoleApp.cpp CMakeLists.txt
git commit -m "CR1: show extensions on student list; ask one student or whole class; bump version to 1.1.0"
git add tests
git commit -m "CR1: add class extension tests; update extension UI test script"
git add docs README.md
git commit -m "Docs: CR1 analysis, release notes, QA evidence for v1.1"
git push -u origin feature/cr1-class-extension
```
On GitHub: open the pull request, wait for the green CI tick, click **Merge pull request**, then create the release:
**Releases > Draft a new release > tag `v1.1`** (and `v1.0` pointing at the earlier commit, if not done yet).
