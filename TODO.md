TODO file adds more words about goals in *RELEASE* file


1. Kernel features registration and track tool

The main purpose of this goal is to automatic run build process throughout
standalone executable.

Developer execute app and select needed features, then app generate script file
to run build with all choose options (-Dfeature options pass in
cmake under the hood)

Moreover, each feature are track by tool: first implementation commit, name,
description, type (NEW, DEPRECATED, DELETED, USE CAREFULLY, RELEASED).
Each feature can dependency on other features, which generates dependency
tree. Working with tree can help to find implementation bugs in feature/s using CI
pipeline. If current kernel configuration falls CI will try disable or swap to the other feature
have on the same level of tree.

2. Unification drivers interaction as modules

Consider drivers are modules. Each module can be loaded or removed in/from the
kernel at run or configure time.

Module contains object instance to interact with device. Object type implement
unify interface of driver type (for instance: hard drive, keyboard, mouse and
etc. types). Each object type associate with specify device. Also module include
driver metadata (driver id number, name, description, type, device id
number).

Kernel must register each module and hold them in the table named **drivers register**.

Come up with a identification device and driver, list devices and driver
modules in kernel.


*Consider point description as reference, not a straight plan.*
