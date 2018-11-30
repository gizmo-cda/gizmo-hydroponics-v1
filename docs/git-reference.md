# Git Reference

- [See Changes](#see-changes)
- [Staging Files](#staging-files)
- [Committing Files](#committing-files)
- [Listing Remotes](#listing-remotes)
- [Pushing Changes](#pushing-changes)
- [Pulling Changes](#pulling-changes)

---

# See Changes

You can see which files have changed, have been delete, or have been added since your last commit:

```bash
git status
```

If you want to see the exact changes in a file:

```bash
git diff <relative-file-path>
```

Note that you will need to replace `<relative-file-path>` with the file you wish to compare. `git` will let you check status in any folder within your repository. All files will be listed relative to your current directory.

# Staging Files

Once you're ready to commit changes, you will need to stage files:

```bash
git add <relative-file-path>
```

If you run `git status` after adding, you will now see which files have been staged. If you want to stage all of your changes:

```bash
git add .
```

If you need to unstage a file:

```bash
git reset -- <relative-file-path>
```

# Committing Files

Once you're happy with the files you've staged, you can commit them along with a comment:

```bash
git commit -m "One-line message here"
```

Note that if you exclude the `-m` option, your current EDITOR will be opened. The first line of the file will be the commit's title. This must be followed by a blank line. The remaining lines following the blank line will be a long description.

# Listing Remotes

You can think of remotes as "servers" or, put another way, machines other than the one you are talking to. Note that I did not say "the machine you are on". You could be using a terminal, logged into a Raspberry Pi, for instance. In that case, the machine you are talking to is the Raspberry Pi.

```bash
git remote -v
```

This output will show you the name of each remote and what it points to. This is how you can figure out which machine that remote references.

# Pushing Changes

You can effectively copy all commits to another machine (a remote):

```bash
git push <remote> <branch>
```

You need to replace `<remote>` with one of the remotes listed from `git remote -v`.

You need to replace `<branch>` with the name of the branch you are working on. If you're not sure of the exact name of the branch, you can see the current branch when you run `git status`. You can also list local branches with `git branch`.

# Pulling Changes

If you've made commits on another machine and wish to bring those to the machine you are working on:

```bash
git pull <remote> <branch>
```

`<remote>` and `<branch>` are the same as described in [Pushing Changes](#pushing-changes).
