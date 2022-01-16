for i in {1..100}
do
    sleep 1
    ./data >in.in
    ./main <in.in >out.out
    python ./test.py <in.in >out1.out
    if ! diff out.out out1.out
    then
        break
    fi
    echo "Correct"
done
