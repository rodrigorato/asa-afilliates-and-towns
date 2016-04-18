make main 

echo "testing public 1"
./cpp.out < tests/public/in1 > tests/public/myout1
diff tests/public/out1 tests/public/myout1

echo "testing public 2"
./cpp.out < tests/public/in2 > tests/public/myout2
diff tests/public/out2 tests/public/myout2

echo "testing public 3"
./cpp.out < tests/public/in3 > tests/public/myout3
diff tests/public/out3 tests/public/myout3

echo "testing public 4"
./cpp.out < tests/public/in4 > tests/public/myout4
diff tests/public/out4 tests/public/myout4

echo "testing public 5"
./cpp.out < tests/public/in5 > tests/public/myout5
diff tests/public/out5 tests/public/myout5

echo "testing public 6"
./cpp.out < tests/public/in6 > tests/public/myout6
diff tests/public/out6 tests/public/myout6


echo "testing enun 1"
./cpp.out < tests/enun/in1 > tests/enun/myout1
diff tests/enun/out1 tests/enun/myout1

echo "testing enun 2"
./cpp.out < tests/enun/in2 > tests/enun/myout2
diff tests/enun/out2 tests/enun/myout2

echo "testing enun 3"
./cpp.out < tests/enun/in3 > tests/enun/myout3
diff tests/enun/out3 tests/enun/myout3

echo "testing enun 4"
./cpp.out < tests/enun/in4 > tests/enun/myout4
diff tests/enun/out4 tests/enun/myout4

