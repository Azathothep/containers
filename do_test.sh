mkdir -p logs
./ft.out > logs/ftlog.txt
./stl.out > logs/stllog.txt

diff logs/ftlog.txt logs/stllog.txt > logs/compare.txt