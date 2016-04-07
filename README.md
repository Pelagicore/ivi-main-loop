
Introduction   
============

This package provides a main loop abstraction, which can be useful to implement main-loop based software components which do not directly rely on a specific main loop implementation. By relying on this abstraction, a package maintainer gives an integrator the freedom to choose the actual main loop framework to use. Having multiple software components relying on the same main loop framework ensures that components can be combined easily and in a reliable way. It also improves code-reuse, which in turn helps improving the overall code quality.

A default implementation of the abstraction is provided using glib's main loop framework, which is one of the popular and widely used main loop frameworks in the OSS.

This software was designed mostly as a proof of concept. As such it is provided "as is." 

Copyright and License information
=================================

Copyright Pelagicore &copy; 2015

The license of this software is MPL 2.0
