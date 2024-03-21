# bed

> [!WARNING]
> I wrote this in half a day, so expect bugs

Embed files into your C applications

This was inspired by one of the [Tsoding](https://github.com/tsoding)
livestreams about [muzializer](https://github.com/tsoding/musializer) where he
creates a application with the same ideia.

## Usage

First you run the bed command to generate the `.c` file

```console
$ ./bed <files>... > resource.c
```
Next, you use the `bed_get()` function, `test.c` is a example on how to do it.

## How to Build

```console
$ make
```

## How to Test

```console
$ make test
```
