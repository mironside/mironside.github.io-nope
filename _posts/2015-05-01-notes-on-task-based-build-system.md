---
layout: post
title: Notes on Task Based Build System
---

- **Line** - A Task name and opaque data object.  Information can only be passed between Tasks through Lines.

- **File** - A physical file on disk.  Files represent Inputs to a Task or Outputs that will be created during Task Execution.  Files may have a list of File dependencies forming a tree (eg. C include files).

- **Task** - Holds a Task-specific opaque data object and has associated Parse and Execute functions.

  - **Parse** - Parses the task data object and returns a list of Input and Output Files.  May also compute Input file dependencies, add them to the Input Files.  Parse does not read any Files.

  - **Execute** - Processes the task using the Task data object: reads Input Files and produces Output Files.
- **MetaTask** - Same as a Task with the addition that it can create new Tasks when Executed.  A MetaTask returns no Outputs when Parsed.

Task dependencies are implicit based on their Inputs and Outputs.  If Task B has an Input File that is also an Output of Task A then Task B depends on Task A.


Let's say we want to use this task system to build a program.  That is, we want to:

1. read a project file to find C source files and target
2. compile all the C source files to obj files
3. link the obj files into an executable

First is the project Task Line:

{% highlight none %}
[project, {ProjectFile: myprogram.project}]
{% endhighlight %}

This declares that this is a project task with a data object.  The task system uses the Project task to Parse the data object producing a list of input and output files.

{% highlight none %}
project
  Input: myprogram.project
  Output:
{% endhighlight %}

The project Parse did not produce any Output files so it is considered at MetaTask and must Execute.  The project Task Executes, reads myprogram.project and finds that the target is game.exe and there are three source files: main.c, game.c and input.c.  It generates three compile Task Lines and one link Task Line which are added to the Task queue.

{% highlight none %}
[compile, {SourceFile: main.c, ObjectFile: main.obj}]
[compile, {SourceFile: game.c, ObjectFile: game.obj}]
[compile, {SourceFile: input.c, ObjectFile: input.obj}]
[link, {ObjectFiles: [main.obj, game.obj, input.obj], Executable: game.exe}]
{% endhighlight %}

The tasks are Parsed to determine the Inputs and Outputs from their Task data object.

{% highlight none %}
compile
  Inputs: main.c
  Outputs: main.obj

compile
  Inputs: game.c
  Outputs: game.obj

compile
  Inputs: input.c
  Outputs: input.obj

link
  Inputs: [main.obj, game.obj, input.obj]
  Outputs: game.exe
{% endhighlight %}

The link Task has Inputs main.obj, game.obj and input.obj which are also Outputs of the three compile Tasks.  Therefore the link Task is implicitly dependent on all three compile Tasks and cannot be run until they complete.

The three compile Tasks do not depend on any other Tasks so they are each Executed.  In fact, since they don't depend on each other they could be Executed in parallel.  Each comile Task compiles it's Input source file and produces it's Output object file.

With all three compile Tasks complete the link Task now has no dependencies.  It Executes taking the three object files as Input and linking them into the Output executable target.

With no more tasks in the queue, the build is complete.