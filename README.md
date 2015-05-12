
Introduction    {#mainpage}
============

This package provides a main loop abstraction, which can be useful to implement main-loop based software components which do not directly rely on a specific main loop implementation. By relying on this abstraction, a package maintainer gives an integrator the freedom to choose the actual main loop framework to use. Having multiple software components relying on the same main loop framework ensures that components can be combined easily and in a reliable way.

A default implementation of the abstraction is provided using glib's main loop framework, which is certainly the most popular and widely used main loop framework in the OSS.

