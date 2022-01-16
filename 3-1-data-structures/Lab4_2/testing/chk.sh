for i in {1..100}
do
    sleep 1
    ./data 100000 5
    ./main 1 raw.in encode.out
    ./main 2 encode.out decode.out
    if ! diff raw.in decode.out
    then
        break
    fi
    echo "Correct"
done
