# 1 -> N
# 2 -> input
# 3 -> threads
# 4 -> strategy

if [[ $4 -eq 4 ]]
then 
    mpirun -np $3 ./mpi.o $1 $2

else 
    ./main $1 $2 $3 $4
fi 