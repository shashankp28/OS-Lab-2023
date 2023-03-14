set xlabel "Number of Frames"
set ylabel "Number of Page Faults"
set title "Page Faults vs Frames"
set term png
set output "output.png"
plot "output1.txt" with linespoints title "File 1", "output2.txt" with linespoints title "File 2"
