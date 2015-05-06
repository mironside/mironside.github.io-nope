---
layout: post
title: Build System in Go
---

I wrote a golang package called tiny/task which implements the ideas of the task based build system described in a [previous post]({% post_url 2015-05-01-notes-on-task-based-build-system %}).  Here is the godoc

{% highlight golang %}
type Line struct {
    Name string
    Data interface{}
}

type Task interface {
    Parse(line interface{}) ([]string, []string, error)
    Execute(line interface{}) ([]Line, error)
}

func Register(name string, task Task)
func Execute(tasklines []Line) error
{% endhighlight %}


Using this package it is very easy to define your own tasks and have tiny/task schedule and execute them.  Here is a program that declares a basic compile task and then runs a compile task that compiles main.c to main.obj.

{% highlight golang %}
package main

import (
	"os/exec"
	"tiny/task"
)

type CompileData struct {
	SourceFile string
	ObjectFile string
}

type CompileTask struct{}

func (*CompileTask) Parse(line interface{}) ([]string, []string, error) {
	d := line.(CompileData)
	return []string{d.SourceFile}, []string{d.ObjectFile}, nil
}

func (*CompileTask) Execute(line interface{}) ([]task.Line, error) {
	d := line.(CompileData)
	cmd := exec.Command("cl.exe", "-Tp", d.SourceFile, "-Fo", d.ObjectFile)
	err := cmd.Run()
	return nil, err
}

func main() {
	task.Register("compile", new(CompileTask))
	taskline := task.Line{"compile", CompileData{"main.c", "main.obj"}}
	task.Execute([]task.Line{taskline})
}
{% endhighlight %}
