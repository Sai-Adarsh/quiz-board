# Concept of branches

I wan't to specify the way I want to use GIT branches in this project.
Normally we have to different types of branches, "permanent" and "temporary"!

## Permanent branches

The default branch is "master" as in every project managed with GIT.

The second branch to mention is develop. This is the branch where all the
development is done. If a new final version is ready for release, the state
of the development brach is merged with into the master branch.

I call these two branches "permanent" branches, because they will never be
deleted.


## Temporary branches

In contrast to permanent branches, there will be so called temporary
branches. These branches are created (most of the time from develop) at some
time for some special reason. After a while, these braches are merged back
to develop or they are deleted. This depends on the task of the development
behind the branch.

Each of these temporary branches will have a fixed prefix in the name.
This prefix reflects the category of the branch.

``fix-`` is used for bug fixes. The remaining part of the branch is either
the issue number of the GIThub project or a descriptive text.

``feat-`` is used for feature branches. Again, a GIThub issue or a
descriptive is used for the rest of the name.

``tmp-`` are simple temporary branches. These branches should be used to
test something. Most of the time these branches should be local and not
pushed to GIThub.
