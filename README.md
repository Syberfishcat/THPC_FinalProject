# Parallel TSP Genetic Algorithm

This repository contains a C++/MPI implementation of a parallel genetic
algorithm for the two-dimensional Traveling Salesman Problem.

## Files

- `tsp.cpp`: parallel MPI genetic algorithm
- `gen_cities.cpp`: random city generator
- `Makefile`: build rules
- `cities.txt`: small example input file

## Build

```bash
make
```

This creates:

```text
tsp
gen_cities
```

## Generate Cities

```bash
./gen_cities 1000 cities_1000.txt 12345
```

## Run

```bash
mpirun -np 4 ./tsp cities_1000.txt 1000 1600 50 0.1
```

Arguments:

```text
./tsp <cities_file> [generations] [total_population] [migration_interval] [mutation_rate]
```

Default values:

```text
generations        = 1000
total_population   = 1600
migration_interval = 50
mutation_rate      = 0.1
```

## Method

Each MPI process evolves a local subpopulation. Every few generations,
the best route is migrated to a neighboring process in a ring topology.
The final global best length is collected with `MPI_Reduce`.
