#!/bin/bash

if [ $# -ne 1 ]
then
    executable_path='./build/bin/dirichlet-problem'
else
    executable_path=$1
fi

# Write system information

# Write header
echo '' > 'results.md'
echo '|Name|Threads count|75^2 nodes|150^2 nodes|300^2 nodes|' >> 'results.md'
echo '|-|-|-|-|-|' >> 'results.md'

# Run executable
$executable_path -seq >> 'results.md'
echo '[  OK  ] Seqential'

$executable_path -omp >> 'results.md'
echo '[  OK  ] OMP'

$executable_path -ompblock >> 'results.md'
echo '[  OK  ] OMP block'

$executable_path -ompwave >> 'results.md'
echo '[  OK  ] OpenMP wave'

mpiexec --use-hwthread-cpus $executable_path -mpi >> 'results.md'
echo '[  OK  ] MPI'

echo 'Done'
