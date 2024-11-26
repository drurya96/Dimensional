# Contributing

Thank you for considering contributing to `Dimensional`! This guide explains how to contribute effectively.

## Workflow

1. **Create a Branch**:
   ```bash
   git checkout -b <issue#>-<custom-description>
   ```
-  Use the <issue#>-<Custom Description> syntax for naming your branch.
-  Example: 42-add-length-dimension.

2. Make Your Changes
- Edit files, write code, and commit your changes
- Use meaningful commit messages:
```bash
git add <changed-files>
git commit -m "Describe what you did"
```
3. Push Your Branch
```bash
git push --set-upstream origin <branch-name>
```

4. Create a Merge Request
- Open a Merge Request in GitLab to merge your branch into `main`
  - Alternatively, if this issue is part of a larger feature, merge into the appropriate feature branch
- Provide a clear description of your changes and link any related issues

## Requirements for Merging

1. Issue Tracking:

- If your feature or fix doesn’t already have an issue, create one and feel free to assign it to yourself.
- Exception: Simple unit or dimension additions do not require an issue.

2. Code Review:

- Merge Requests must be reviewed and approved by a maintainer before merging.

3. Automated Pipelines:

- All pipelines must pass successfully for the Merge Request to be merged.
- Ensure tests are written and pass for any new features or fixes.

## Code Guidelines

- Follow the existing code style and conventions used in the repository.
- Ensure all changes are properly documented (e.g., new units, dimensions, or examples).
- Avoid introducing unnecessary dependencies.

## Testing
- Run tests locally before pushing:
- Add unit tests for any new features or bug fixes.

## Helpful Tips

- Communication: If you're unsure about your changes, open a Draft Merge Request or discuss in the issue.
- Small Changes: For simple fixes or additions, keep Merge Requests focused and small to simplify review.
- Documentation: If your change impacts usage, update relevant documentation files (e.g., README.md, Usage.md).

**Thank you for contributing to `Dimensional`!**
