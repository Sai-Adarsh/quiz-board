# Concept of branches

I wan't to specify the way I want to use GIT branches in this project.
Normally we have two different types of branches, "permanent" and "temporary"!
While "permanent branches" have a unlimited lifespan, the lifespan of
"temporary branches" is limited (in most cases).


## Permanent branches

The default branch is ``master`` as in every project managed with GIT.

The second branch to mention is ``develop``. This is the branch where all the
development is done. If a new final version is ready for release, the state
of the development branch is the base for a release (see release branches below).

I call these two branches "permanent" branches, because they will never be
deleted.

Neither ``master`` nore ``develop`` should be used for simple *checkpoint
commits*. While ``master`` should only be merged with *release branches*,
the development should be done within the *temporary branches*. Depending on
the type of this temporary branch, they are merged back i.e. to ``develop``.


## Temporary branches

In contrast to *permanent branches*, there are so called *temporary branches*.
These branches are created (most of the time from ``develop``) at some
time for some special reason. After a while, these branches are merged back
to develop or they are deleted. This depends on the task of the development
behind the branch.

Each of these temporary branches will have a well defined prefix in the name.
This prefix reflects the category of the branch.

* ``release-`` is used for *release branches*. If the development is
ready for a new version of the code, a *release branch* is split of of the
*develop branch*. If this *release candidate* is ready to be the new
version, this branch is merged as final code into master and into develop.
This way, fixes done within the release cicle are merged back to develop.

* ``fix-`` is used for *bug fix branches*. The remaining part of the branch
is either the issue number of the GIThub project or a descriptive text.

* ``feat-`` is used for *feature branches*. Again, a GIThub issue or a
descriptive is used for the rest of the name.

* ``tmp-`` are *simple temporary branches*. These branches should be used to
test something. Most of the time these branches should be local and not
pushed to GIThub.
