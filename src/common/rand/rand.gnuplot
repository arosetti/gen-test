set terminal postscript portrait enhanced color dashed lw 1 "Helvetica" 14

set grid
show grid
set output "rand.eps"

set format x "%3.2f"
set format y "%3.2f"
set size square

set style function dots
set title "random numbers test" 
set xrange [ 0.00000 : 1.00000 ] noreverse nowriteback
set yrange [ 0.00000 : 1.00000 ] noreverse nowriteback

plot 'rand.dat' using 1:2

clear
set output "gaussian.eps"
set title "gaussian random numbers test" 
set xrange [ -6.00000 : 6.00000 ] noreverse nowriteback
set yrange [ -6.00000 : 6.00000 ] noreverse nowriteback
plot 'gaussian.dat' using 1:2
