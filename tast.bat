::中心x,中心y,半径,線幅,変調率,極対数,データ数,本数,細線幅
::center position x, center position y, radius, pitch of concentric circles, modulation ratio, number of pole pairs, number of data, number of concentric circles, width of line
test.exe 50 50 20 1 0.999 7 250 4 0.08
del test1.txt
ren test.txt test1.txt
test.exe 50 50 10 1 0.999 1 150 4 0.08
del test2.txt
ren test.txt test2.txt
copy head.txt + test1.txt + test2.txt + tail.txt pattern-A.grb
pause
