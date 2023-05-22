#!/bin/bash

for n in 1 2 4 8; do
	DIRNAME="v2_processes_${n}"
	mkdir -p "$DIRNAME"
	SCRIPTNAME="v2_run_processes_${n}.sh"
	cat <<EOF > "${DIRNAME}/${SCRIPTNAME}"
#!/bin/bash
#PBS -l walltime=00:05:00
#PBS -l select=1:ncpus=${n}:mpiprocs=${n}
#PBS -m n
cd \$PBS_O_WORKDIR
MPI_NP=\$(wc -l \$PBS_NODEFILE | awk '{ print \$1 }')
echo "Number of MPI process: \$MPI_NP"
echo "File \$PBS_NODEFILE:"
cat \$PBS_NODEFILE
echo
mpirun -machinefile \$PBS_NODEFILE -np \$MPI_NP ./build_v2/lab1 10000 0.00000001 0.00001 >> "${DIRNAME}/result_processes_${n}"
EOF
	for i in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15; do
		qsub "${DIRNAME}/${SCRIPTNAME}"
	done
done

for n in 6 8; do
        DIRNAME="v2_processes_double_${n}"
        mkdir -p "$DIRNAME"
        SCRIPTNAME="v2_run_processes_double_${n}.sh"
        cat <<EOF > "${DIRNAME}/${SCRIPTNAME}"
#!/bin/bash
#PBS -l walltime=00:03:00
#PBS -l select=2:ncpus=${n}:mpiprocs=${n}
#PBS -m n
cd \$PBS_O_WORKDIR
MPI_NP=\$(wc -l \$PBS_NODEFILE | awk '{ print \$1 }')
echo "Number of MPI process: \$MPI_NP"
echo "File \$PBS_NODEFILE:"
cat \$PBS_NODEFILE
echo
mpirun -machinefile \$PBS_NODEFILE -np \$MPI_NP ./build_v2/lab1 10000 0.00000001 0.00001 >> "${DIRNAME}/result_processes_${n}"
EOF
        for i in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15; do
                qsub "${DIRNAME}/${SCRIPTNAME}"
        done
done
