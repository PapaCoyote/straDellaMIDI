# Branch Merging Guide - straDella MIDI Project

## Table of Contents
1. [Overview](#overview)
2. [Branch Strategy](#branch-strategy)
3. [Merging Workflows](#merging-workflows)
4. [Merge Strategies](#merge-strategies)
5. [GitHub Pull Request Workflow](#github-pull-request-workflow)
6. [Troubleshooting](#troubleshooting)
7. [Best Practices](#best-practices)

## Overview

This guide explains how to merge branches in the straDella MIDI project. Whether you're working on features, bug fixes, or experiments, understanding how to properly merge your work is essential for maintaining a clean and functional codebase.

## Branch Strategy

### Main Branches
- **`main`**: The production-ready branch. All code here should be stable and tested.
- **Feature branches**: Named like `feature/your-feature-name` or `copilot/feature-description`
- **Bug fix branches**: Named like `fix/bug-description`
- **Experimental branches**: Named like `experiment/idea-name`

### Current Project Structure
The straDella MIDI project currently uses:
- `main` - stable, production code
- Feature branches for development work

## Merging Workflows

### 1. Fast-Forward Merge (Simplest)

**When to use**: When your feature branch is based on the latest `main` and there are no conflicts.

```bash
# Ensure you're on the target branch (main)
git checkout main

# Pull the latest changes
git pull origin main

# Merge your feature branch
git merge feature/your-feature-name

# Push to remote
git push origin main
```

**Result**: Creates a linear history with no merge commit.

### 2. Merge with Merge Commit (Recommended for Features)

**When to use**: For feature branches where you want to preserve the branch history.

```bash
# Ensure you're on main
git checkout main

# Pull latest changes
git pull origin main

# Merge with --no-ff to create a merge commit
git merge --no-ff feature/your-feature-name

# Edit the merge commit message if needed
# Push to remote
git push origin main
```

**Result**: Creates a merge commit that shows when the feature was integrated.

### 3. Squash Merge (Clean History)

**When to use**: When your feature branch has many small commits that you want to combine into one.

```bash
# Ensure you're on main
git checkout main

# Squash merge
git merge --squash feature/your-feature-name

# Commit the squashed changes
git commit -m "Add feature: your feature description"

# Push to remote
git push origin main
```

**Result**: Combines all commits from the feature branch into a single commit.

### 4. Rebase and Merge (Advanced)

**When to use**: When you want to maintain a linear history and incorporate the latest changes from main.

```bash
# Switch to your feature branch
git checkout feature/your-feature-name

# Rebase onto main
git rebase main

# If there are conflicts, resolve them, then:
git rebase --continue

# Switch back to main
git checkout main

# Fast-forward merge
git merge feature/your-feature-name

# Push to remote
git push origin main
```

**Result**: Creates a linear history by replaying your commits on top of main.

## Merge Strategies

### Which Strategy to Choose?

| Strategy | Pros | Cons | Use When |
|----------|------|------|----------|
| **Fast-Forward** | Simplest, linear history | No record of branch | Small, quick changes |
| **Merge Commit** | Preserves history, clear integration points | More complex history | Feature development |
| **Squash** | Clean history, single commit | Loses individual commit history | Many small commits |
| **Rebase** | Linear history, clean | Rewrites history, can be complex | Want to avoid merge commits |

### Recommended for straDella MIDI

For this project, we recommend:
- **Feature branches**: Use merge commit (`--no-ff`)
- **Bug fixes**: Use squash merge
- **Experimental work**: Use squash merge or regular merge

## GitHub Pull Request Workflow

### Creating a Pull Request

1. **Push your branch to GitHub**:
   ```bash
   git push origin feature/your-feature-name
   ```

2. **Open a Pull Request**:
   - Go to https://github.com/PapaCoyote/straDellaMIDI
   - Click "Pull requests" → "New pull request"
   - Select your branch
   - Fill in the title and description
   - Click "Create pull request"

3. **Review Process**:
   - Wait for code review
   - Address any feedback
   - Push additional commits if needed

4. **Merge the PR**:
   - Once approved, click "Merge pull request"
   - Choose merge strategy (Create a merge commit, Squash and merge, or Rebase and merge)
   - Confirm the merge

### Merging a Pull Request via GitHub

**Option 1: Create a merge commit**
- Preserves all commits from the branch
- Best for feature branches

**Option 2: Squash and merge**
- Combines all commits into one
- Best for bug fixes or small features

**Option 3: Rebase and merge**
- Rebases commits onto main
- Creates linear history

### Merging a Pull Request via Command Line

```bash
# Fetch the branch
git fetch origin feature/your-feature-name

# Checkout main
git checkout main

# Merge the PR branch
git merge --no-ff origin/feature/your-feature-name

# Push to GitHub
git push origin main
```

## Troubleshooting

### Merge Conflicts

When Git can't automatically merge changes, you'll get a merge conflict.

**Resolving conflicts**:

1. **Start the merge**:
   ```bash
   git merge feature/your-feature-name
   # CONFLICT message appears
   ```

2. **Check conflicted files**:
   ```bash
   git status
   # Look for "both modified" files
   ```

3. **Open conflicted files**:
   Files will contain conflict markers:
   ```cpp
   <<<<<<< HEAD
   // Code from main branch
   =======
   // Code from your branch
   >>>>>>> feature/your-feature-name
   ```

4. **Resolve the conflict**:
   - Edit the file to keep what you want
   - Remove the conflict markers (`<<<<<<<`, `=======`, `>>>>>>>`)
   - Save the file

5. **Mark as resolved**:
   ```bash
   git add filename.cpp
   ```

6. **Complete the merge**:
   ```bash
   git commit
   ```

### Aborting a Merge

If you want to cancel a merge:
```bash
git merge --abort
```

### Checking Merge Status

```bash
# See what would be merged
git diff main..feature/your-feature-name

# See commits that would be merged
git log main..feature/your-feature-name

# See affected files
git diff --name-only main..feature/your-feature-name
```

## Best Practices

### Before Merging

1. **Ensure your branch is up to date**:
   ```bash
   git checkout feature/your-feature-name
   git pull origin main
   ```

2. **Run tests**:
   - Build the project in Projucer
   - Test MIDI functionality
   - Verify keyboard input works
   - Check visual feedback

3. **Review your changes**:
   ```bash
   git diff main
   ```

4. **Check for untracked files**:
   ```bash
   git status
   ```

### During Merging

1. **Write clear merge commit messages**:
   ```
   Merge feature/keyboard-velocity-control
   
   - Added velocity sensitivity based on key press duration
   - Updated MIDI output to use variable velocity
   - Added UI controls for velocity adjustment
   ```

2. **Test after merging**:
   Always test the merged code to ensure nothing broke

3. **Keep merge commits focused**:
   One feature or fix per merge

### After Merging

1. **Delete merged branches**:
   ```bash
   # Delete local branch
   git branch -d feature/your-feature-name
   
   # Delete remote branch
   git push origin --delete feature/your-feature-name
   ```

2. **Update your local main**:
   ```bash
   git checkout main
   git pull origin main
   ```

3. **Tag releases** (when appropriate):
   ```bash
   git tag -a v1.0.0 -m "Release version 1.0.0"
   git push origin v1.0.0
   ```

## Common Scenarios for straDella MIDI

### Scenario 1: Merging a New Feature

**Example**: You've added sustain pedal support in a feature branch.

```bash
# Ensure main is up to date
git checkout main
git pull origin main

# Merge your feature
git merge --no-ff feature/sustain-pedal

# Test the merged code
# Build in Projucer, test MIDI output

# Push to GitHub
git push origin main

# Delete the feature branch
git branch -d feature/sustain-pedal
git push origin --delete feature/sustain-pedal
```

### Scenario 2: Merging a Bug Fix

**Example**: Fixed a bug where some keys weren't responding.

```bash
# Squash merge to keep history clean
git checkout main
git merge --squash fix/unresponsive-keys

# Commit with descriptive message
git commit -m "Fix: Resolve unresponsive keys issue

- Fixed KeyListener event handling
- Added proper key state tracking
- Tested all keyboard rows"

# Push to GitHub
git push origin main
```

### Scenario 3: Updating Your Feature Branch with Latest Main

**Example**: Main has new changes you want in your feature branch.

```bash
# Switch to your feature branch
git checkout feature/your-feature

# Merge main into your feature branch
git merge main

# Or, if you prefer rebasing:
git rebase main

# Resolve any conflicts
# Push your updated branch
git push origin feature/your-feature
```

### Scenario 4: Merging via GitHub PR

**Example**: Using GitHub's web interface.

1. Push your branch:
   ```bash
   git push origin feature/new-gui-theme
   ```

2. Go to GitHub and create a PR

3. Wait for review/approval

4. On GitHub, click "Squash and merge" or "Create a merge commit"

5. Locally, update your main:
   ```bash
   git checkout main
   git pull origin main
   ```

## JUCE Project Considerations

### Build Files

When merging branches in the straDella MIDI project:

1. **Don't commit build artifacts**:
   - `Builds/` folder (except the generated project files if using Projucer)
   - Compiled binaries
   - `.DS_Store` files

2. **Update .gitignore**:
   Already configured in the project to exclude:
   - `Builds/MacOSX/build/`
   - `Builds/VisualStudio*/x64/`
   - `*.o`, `*.obj`, etc.

3. **Projucer file conflicts**:
   If `straDellaMIDI.jucer` has conflicts:
   - Usually accept the version with new source files
   - Re-open in Projucer and re-save to regenerate build files

### Source Files

When merging C++ source files:
- Pay attention to header includes
- Watch for conflicting class definitions
- Ensure JUCE module dependencies are consistent

## Quick Reference Commands

```bash
# Check current branch
git branch

# See what would be merged
git diff main..feature-branch

# Merge with commit
git merge --no-ff feature-branch

# Squash merge
git merge --squash feature-branch && git commit

# Abort merge
git merge --abort

# Delete merged branch
git branch -d feature-branch

# Push changes
git push origin main

# Update main
git checkout main && git pull origin main
```

## Getting Help

If you encounter issues:

1. **Check Git status**: `git status`
2. **View Git log**: `git log --oneline --graph --all`
3. **See diff**: `git diff`
4. **Git documentation**: https://git-scm.com/doc
5. **JUCE forum**: https://forum.juce.com/

## Summary

For the straDella MIDI project:

1. ✅ Use feature branches for new work
2. ✅ Keep `main` stable and tested
3. ✅ Use merge commits for features (`--no-ff`)
4. ✅ Use squash merges for bug fixes
5. ✅ Test before and after merging
6. ✅ Delete merged branches
7. ✅ Write clear commit messages

Happy merging! 🎹🎵
