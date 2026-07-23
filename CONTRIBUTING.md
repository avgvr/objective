## Table of content

- [I have a question](#i-have-a-question)
- [I want to contribute](#i-want-to-contribute)
- [Style guide](#style-guide)
- [Git branching workflow](#git-branching-workflow)
- [Sending a patch](#sending-a-patch)
- [Suggesting enhancement](#suggesting-enhancement)
- [Reporting a bug](#reporting-a-bug)
- [Commit message](#commit-message)
- [Discussion lists](#discussion-lists)

## I have a question
**Before to ask a question, ensure that information already has in markdown files**

It is best to search for existing issue on github or topic in discussion list.
In case you have a suitable issue/topic and still need clarification, write your
question there.

If you then still feel the need to ask a question and need clarification, we recommend the following:

- Open issue/topic.
- Provide as much context as you can about what you are running into.

We will then take care of the issue an soon as possible.

## I want to contribute
You need to find an idea firstly. Look through open issues, discussions and bugs lists or [TODO.md](TODO.md) file.
Try to build a project from `develop` or `release` branch and debug kernel to
find some bugs. [BUILD.md](BUILD.md) file contains instruction to build a source
code. As a non expert coder is is a hugely useful way to contribute to a
project.

When you find or already have ideas, not to rush write a code.

Ask yourself these questions firstly:

- What is the kind of problem i am trying to solve?
- What are the exact semantics/behaviours?
- How it affect on others system components and kernel at all?
- It is enough at this level of details? Or need dive deep?

And publish your thoughts in corresponding discussions or
bugs list to except waste a time to write irrelevant code.

**Push the patch directly if it solve an obvious problem.**

See [suggesting enhancement](#suggesting-enhancement) or [reporting a bug](#reporting-a-bug) section.

Once solution is reached, write a code and push patch to review.
See [style guide](#style-guide) and [sending a patch](#sending-a-patch) sections.

## Style guide
Use google code style (unfinished section)

## Git branching workflow
This model branching is based on [Gitflow workflow](https://www.atlassian.com/git/tutorials/comparing-workflows/gitflow-workflow)
except standalone `feature` branch intend to request to create a new feature
branch and no release branches (they are replaced by tags in `main` branch).
Bug branches are similar to feature branches in workflow, but purposes are
differ. `email` branch indent to receive patches from email and used by gerrit
instance when pushing to review.

## Sending a patch
**`<builddir>/CMakeDexport.txt` file must is attached to the commit**

Before to submit a patch, please read its whole. Make sure there are no unintended changes and fix them.
Trailing whitespaces, empty commits, test code, bad words and etc. must be deleted.

Pay attention to a commits count. Stick to following rule: the less the better.
One commit is most preferable. Two and more commits must be divided by particular patches.
Important to send a good readable patch using comments, appropriate variable and
functions names, break the code into semantic parts and etc.

Choose the proper branch to merge. Standalone `bug` and `feature` branches using to create a not
present issue in repository. Specific feature or `bugfix*` refer to a present
issue. `email`, `main` and `develop` branches are autonomics, and used not for to submit a changes.

Make sure that the patch includes tests to check updates work properly. Try to
accomplish greatest code coverage.

Supply a patch with following information; Do not format your description like
you are filling out a form.

- Name of topic in discussions or bugs list
- What patch does in a short paragraph.
- Which branch the patch is against.
- Which subsystem and module have touched.
- Describe the effect your patch has on performance, if any.
- Include documentation on how to use a new function with examples.

You can submit updates three ways: gerrit instance, Github PR or email. Gerrit
is most prefer method. Github and email are less desirable. All links you can
find in [README.md](README.md) file in link section.

All received patches are send to a gerrit instance and reviewed. Verification run by
CI/CD server. Stages ran on the server: kernel configuration, source code building and tests execution. If no error while stages running, verification passed.
Patch might be returned if verification or code review not passed. CI/CD
provides a link to a executed pipeline where you can get acquainted with
detailed log on a patch page. Or get a feedback if code review is failed.

Apply required changes and resubmit the patch to gerrit to resume review
process. Resubmit the patch from github or email is not allowed now.

## Suggesting enhancement
Step following points to submit a good enhancement suggestion:

- Use a clear and descriptive title for the issue to identify the suggestion.
- Provide a step-by-step description of the suggested enhancement in as many details as possible.
- Describe the current behavior and explain which behavior you expected to see instead and why. At this point you can also tell which alternatives is losing.
- Explain why this enhancement would be useful to most CONTRIBUTING.md users. You may also want to point out the other projects that solved it better and which could serve as inspiration.

Create a new topic in discussions list or add your thoughts if topic is already
created. **Please be kind and patient to all community members.**

Enhancement suggestions are tracked in discussion lists.

## Reporting a bug
When you spot the problem, it is a good practice to report this. I'm trying to
provide description how to report a bug in a right way, that help me to fix a
error pretty soon.

Before submitting a bug report determine if your bug is a bug and not a error
on your side e.g. using incompatible tools/versions. Ensure that the report has
a fully information to not a chase you up for more details. Get a much useful
information into your message as you can. The developer will need to reproduce
your problem before try and fix it. Do not just say "It is broken" or "It
crashed".

Attach the following information:

- Version of the kernel.
- Explain the behavior you would expect and the actual behaviour.
- Exactly what steps did you take to trigger the bug?
- `<builddir>/CMakeDexport.txt` file content.
- If possible provide an exact example that demonstrates your problem. This
could be a code snippet or display content from emulator or something else.
Create the minimum working example that demonstrates your problem. Remove
anything that is unnecessary as this might introduce different problems or
obscure the issue.

Also check to see if the problem has been reported before. This could mean
searching for resolved problems as well as unresolved ones.

If you find your bug already reported, but you can provide more information on
the problem, then add this to the discussion as it will help the developer solve
it.

You can use bugs mailing list or github issues to report a bug.

## Commit message
Use the following [format](https://wiki.openstack.org/wiki/GitCommitMessages) to
write a pretty commit message. Descriptive and structured message is better to
allow to build a representation of changes in mind. Do not use short messages
consists of a only header. Developer is not to try a solve puzzles if a good message
is not provides.

## Email lists
There are three email list current. `Discussions list` needs to share a
enhancements with others developers. `Bugs list` uses to publish findings bugs.
And last list is `patches` to send source code changes.

Anyone can to write and read someone list without an invitation.
Any activity is welcome :)
