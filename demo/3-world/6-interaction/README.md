# Basic Usage

## Ubuntu

```bash
./test.out 11 120000
```

## Windows

```bash
./test.exe 11 120000
```

The first two arguments are seed parameters. Minor changes to these parameters will result in a new terrain layout at the start of simulation. Running the simulation twice with same parameters will produce the same terrain layout.

For best results, we recommend keeping seed parameters at the same orders of magnitude shown above. 

If arguments are not specified, a set of default seed parameters will be used. This will run a simulation starting from a default world that's used to test how changes to code affect simulation behavior.

On Windows, you can simply click the executable. This will run a simulation starting from the default world.