---
layout: post
title: Notes on Task Based Build System
---

- Line - a Task name and opaque data object that declare all task information.  data is passed between tasks through Lines.

- File - a physical file on disk.  Files can contain a list of File dependencies.  Files may also store an Error if they could not be built.

- Task
  - Parse - parses the task data object and returns a list of Input and Output files.  may also compute Input file dependencies and add them to the input Files.  Parse is does not read any files.  if Parse returns no Output files it is considered to be a MetaTask that will create new Tasks.  If Parse does return Outputs it is assumed that it will not create new tasks on Execute and will not change the task graph.

  - Execute - processes the task using the task data object.  can read inputs and write output files.  if an error occurs when producing an output File the error is noted on the output File.  input and outputs are crc'd and stored during a build.  If crc's for all inputs and outputs for this task match the previous build nothing has changed and the Execute step is skipped.  MetaTasks may create new tasks during Execution by returning Task Lines.

Tasks dependencies are implicit based on their Inputs and Outputs.  If Task B has an Input File that is also an Output of Task A then Task B depends on Task A.


Let's say we want to use this task system to build a program.  That is, we want to:

1. read a project file to find C source files and target
2. compile all the C source files to obj files
3. link the obj files into an executable

First is the project Task Line:

```
[project, {ProjectFile: myprogram.project}]
```

This declares that this is a project task with a data object.  The task system uses the Project task to Parse the data object producing a list of input and output files.

```
project
  Input: myprogram.project
  Output:
```

The project Parse did not produce any Output files so it is considered at MetaTask and must Execute.  The project Task Executes, reads myprogram.project and finds that the target is game.exe and there are three source files: main.c, game.c and input.c.  It generates three compile Task Lines and one link Task Line which are added to the Task queue.

```
[compile, {SourceFile: main.c, ObjectFile: main.obj}]
[compile, {SourceFile: game.c, ObjectFile: game.obj}]
[compile, {SourceFile: input.c, ObjectFile: input.obj}]
[link, {ObjectFiles: [main.obj, game.obj, input.obj], Executable: game.exe}]
```

The tasks are Parsed to determine the Inputs and Outputs based on their Task data object.

<code>
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
</code>

The link Task has Inputs main.obj, game.obj and input.obj which are also Outputs of the three compile Tasks.  Therefore the link Task is implicitly dependent on all three compile Tasks and cannot be run until they complete.

The three compile Tasks do not depend on any other Tasks so they are each Executed.  In fact, since they don't depend on each other they could be Executed in parallel.  Each comile Task compiles it's Input source file and produces it's Output object file.

With all three compile Tasks complete the link Task now has no dependencies.  It Executes taking the three object files as Input and linking them into the Output executable target.

With no more tasks in the queue, the build is complete.