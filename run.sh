# 1 -> N
# 2 -> input
# 3 -> threads
# 4 -> strategy

if [[ $4 -eq 0 ]]
then 
    ./strategy0 $1 $2 $3

elif [[ $4 -eq 1 ]]
then 
    ./strategy1 $1 $2 $3

elif [[ $4 -eq 2 ]]
then 
    ./strategy2 $1 $2 $3

elif [[ $4 -eq 3 ]]
then 
    ./strategy3 $1 $2 $3

elif [[ $4 -eq 4 ]]
then 
    mpirun -np $3 ./mpi.o $1 $2

else 
    echo "Invalid Strategy number!"
fi 