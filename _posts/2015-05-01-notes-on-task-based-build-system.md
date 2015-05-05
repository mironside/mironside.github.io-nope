---
layout: post
title: Notes on Task Based Build System
---

- **Line** - Tasks are defined by Lines which declare the Task name and Task-specific data.  Information can only be passed between Tasks through Lines.

- **File** - Represents a physical file on disk that may be an Input or an Output of a Task.  A File can have a list of File Dependencies, forming a tree.

  - **FindDependencies** - A File may have an associated function to find it's dependencies.  For instance, a .c File would have a function to find Files it #includes.

- **Task** - A Task consists of it's opaque data object and Parse and Execute functions which perform the actual logic of the Task.

  - **Parse** - Parses the task data object and returns the list of Input and Output Files.  Parse cannot read Input Files, it only translates the task-specific data into general inputs and outputs used for scheduling.

  - **HasChanges** - Determines if any Input, Input Dependencies or Output Files have changed.  If there are no changes the Task does not need to Execute.

  - **Execute** - Processes the task using the Task data object: reads Input Files and produces Output Files.

- **MetaTask** - Same as a Task with the addition that it can return new Task Lines when Executed.  A MetaTask returns no Outputs when Parsed.

Task dependencies are implicit based on their Inputs and Outputs.  If Task B has an Input File that is also an Output of Task A then Task B depends on Task A.  That is, B cannot Execute until A has completed.


Example
-------
Let's say we want to use this task system to build a program.  We want to:

1. Read a project file to find C source files and target
2. Compile all the C source files to obj files
3. Link the obj files into an executable

First is the project Task Line:

{% highlight none %}
[project, {ProjectFile: myprogram.project}]
{% endhighlight %}

This declares the project Task and it's data.  The Task system calls Parse on the data object producing a list of input and output files.

{% highlight none %}
project
    Input: myprogram.project
    Output:
{% endhighlight %}

The project Parse did not produce any Output files so it is considered a MetaTask and must Execute.

The project Task Executes reading myprogram.project and finds that the target is game.exe and there are three source files: main.c, game.c and input.c.  It creates three compile Task Lines and one link Task Line which are added to the Task queue.

{% highlight none %}
[compile, {SourceFile: main.c, ObjectFile: main.obj}]
[compile, {SourceFile: game.c, ObjectFile: game.obj}]
[compile, {SourceFile: input.c, ObjectFile: input.obj}]
[link, {ObjectFiles: [main.obj, game.obj, input.obj], Executable: game.exe}]
{% endhighlight %}

These new Tasks are Parsed to determine the Inputs and Outputs from their Task data object.

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

The link Task uses the files main.obj, game.obj and input.obj as Inputs which are also the Outputs of the compile Tasks.  Therefore the link Task is implicitly dependent on all three compile Tasks and cannot be run until they complete.

The three compile Tasks do not depend on any other Tasks so they are each Executed.  Since they don't depend on each other they could Execute in parallel.  Each comile Task compiles it's Input source file and produces it's Output object file.

With all three compile Tasks complete the link Task now has no dependencies.  It Executes linking the three object file Inputs into the game.exe Output file.

With no more tasks in the queue, the build is complete.