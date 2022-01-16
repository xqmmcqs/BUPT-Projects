for i in {1..100}
do
    sleep 1
    ./data 1000 1500 >in.in
    ./main <in.in >out.out
    ./floyd <in.in >outt.out
    if ! diff out.out outt.out
    then
        break
    fi
    echo "Correct"
done
