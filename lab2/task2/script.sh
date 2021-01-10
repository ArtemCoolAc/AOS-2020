son=0
father=0
for ((a = 0; a < 1000; a++))
do
  result=$(./t2)
  if [ "$result" = "Son" ]; then
    son=$((son + 1))
  else
    father=$((father + 1))
  fi
done
echo "Son is $son, father is $father"
