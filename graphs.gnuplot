# Gnuplot script file

set terminal postscript portrait enhanced color dashed lw 1 "Helvetica" 14

set grid
show grid
set size 2, 1

set key bottom right
set pointsize 2
set size square
#--------------------------------------------------------------

set xlabel "Generations" font "Helvetica"
#set xtic 1 10
set mytics 2
set ytic 0.05 0.05
set yrange [0:1]
set style line 1 lt 1 lw 5 lc 1

set output "graphs/fitness.eps"
set ylabel "Fitness" font "Helvetica"
plot 'logs/best_individual_fitness.log' title "best individual fitness" w lines lw 5 , 'logs/worst_individual_fitness.log' title "worst individual fitness" w lines lc 2 lw 1, 'logs/avg_fitness.log' title "average fitness" w lines lc 4 lw 3

clear

set output "graphs/fault_coverage.eps"
set ylabel "Fault Coverage" font "Helvetica"
plot 'logs/best_individual_fault_coverage.log' title "best individual fault coverage" w lines lw 5 , 'logs/max_fault_coverage.log' title "max fault coverage" w lines lc 3 lw 3 , 'logs/worst_individual_fault_coverage.log' title "worst individual fault coverage" w lines lc 2 lw 1, 'logs/avg_fault_coverage.log' title "average fault coverage" w lines lc 4 lw 3

clear

set output "graphs/mutation_rate.eps"
set ylabel "Mutation Rate" font "Helvetica"
plot 'logs/mutation_rate.log' title "mutation rate" w lines lw 3

clear
set autoscale
set ytic 1 5

set output "graphs/chromosome_length.eps"
set ylabel "Chromosome Length" font "Helvetica"
plot 'logs/best_individual_chromosome_length.log' title "best individual chromosome length" w lines lw 5 , 'logs/max_chromosome_length.log' title "max chromosome length" w lines lt 1 lw 3 lc 3 , 'logs/worst_individual_chromosome_length.log' title "worst individual chromosome length" w lines lt 1 lw 1 lc 2, 'logs/avg_chromosome_length.log' title "average chromosome length" w lines lt 1 lw 3 lc 4
