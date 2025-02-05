# include "common.h"
# include "gnuplot_functions.h"

#define GNUPLOT "gnuplot --persist"

const char *data_path = "../data/test/";
//extern char *ic_label;

void plot_gnuplot_time_series(char png_name[], double parameter, int iter)
{	
	FILE *gp;
	gp = popen(GNUPLOT, "w");
	fprintf(gp, "set terminal png enhanced size 2500,1500 font \"CMR10\" 60 \n");
	fprintf(gp, "set colors classic \n");
	fprintf(gp, "set autoscale yfix \n");
	fprintf(gp, "set autoscale xfix \n");

	// fprintf(gp, "unset key \n");
	// fprintf(gp, "unset cbtics \n");
	// fprintf(gp, "unset xtics \n");
	// fprintf(gp, "unset ytics \n");
	// fprintf(gp, "unset colorbox \n");
	// fprintf(gp, "unset border \n");
	// fprintf(gp, "set lmargin 0 \n");
	// fprintf(gp, "set rmargin 0 \n");
	// fprintf(gp, "set tmargin 0 \n");
	// fprintf(gp, "set bmargin 0 \n");

	fprintf(gp, "set title \"Time series test\\n{/*0.7 Control Parameter=%.5f; Max Iterations =%d;}\" \n", parameter, iter); 

	//fprintf(gp, "set format y '' \n");
	//fprintf(gp, "set cbtics 0,500,1000 \n");
	//fprintf(gp, "set palette maxcolors 4\n");
	//fprintf(gp, "set xrange[0:6.28] \n");
	//fprintf(gp, "set yrange[0.25:0.35] \n");
	fprintf(gp, "set output '%s'\n", png_name);

	fprintf(gp, "plot 'test_phase_space.dat' u 0:2:0 w l lw 2 palette noti\n");
	//fprintf(gp, "pause -1");
	fclose(gp);
}

void plot_gnuplot_RP_matrix(char filename[], int N)
{	
	FILE *gp = popen(GNUPLOT, "w");
    if (gp == NULL)
    {
        fprintf(stderr, "Error opening pipe to gnuplot.\n");
        return;
    }
    fprintf(gp, "set terminal png size %d,%d \n", N, N);
    fprintf(gp, "set size square \n");
	fprintf(gp, "unset key \n");
	fprintf(gp, "unset xtics \n");
	fprintf(gp, "unset ytics \n");
	fprintf(gp, "unset cbtics \n");
	fprintf(gp, "unset colorbox \n");
	fprintf(gp, "unset border \n");
	fprintf(gp, "set lmargin 0 \n");
	fprintf(gp, "set rmargin 0 \n");
	fprintf(gp, "set tmargin 0 \n");
	fprintf(gp, "set bmargin 0 \n");
	//fprintf(gp, "set autoscale yfix \n");
	//fprintf(gp, "set autoscale xfix \n");
    fprintf(gp, "set xrange [-0.5:%d.5] \n", N-1);
    fprintf(gp, "set yrange [%d.5:-0.5] \n", N-1);
    fprintf(gp, "set palette maxcolors 2\n");
    fprintf(gp, "set palette defined (0 'white', 1 'black')\n");
    fprintf(gp, "set output '%s'\n", filename);
	//fprintf(gp, "plot 'rp.dat' w p pt 5 ps 0.2 lc 'black' notitle\n");
    fprintf(gp, "plot 'rm.dat' matrix w image\n");
    fclose(gp);
}

void plot_gnuplot_RPs_no_title(char filename[], int N)
{
	FILE *gp;
	gp = popen(GNUPLOT, "w");
	fprintf(gp, "set terminal png enhanced crop size 2500,2500 font \"CMR10\" 50 \n");
	fprintf(gp, "set size square \n");
	fprintf(gp, "unset key \n");

	fprintf(gp, "unset key \n");
	fprintf(gp, "unset cbtics \n");
	fprintf(gp, "unset xtics \n");
	fprintf(gp, "unset ytics \n");
	fprintf(gp, "unset colorbox \n");
	fprintf(gp, "unset border \n");
	fprintf(gp, "set lmargin 0 \n");
	fprintf(gp, "set rmargin 0 \n");
	fprintf(gp, "set tmargin 0 \n");
	fprintf(gp, "set bmargin 0 \n");

	fprintf(gp, "unset colorbox \n");
	fprintf(gp, "set yrange[0:%d] \n",N);
    fprintf(gp, "set xrange[0:%d] \n",N);
	//fprintf(gp, "set xlabel 'i' \n");
	//fprintf(gp, "set ylabel 'j' \n");
	//fprintf(gp, "set format y '' \n");
	//fprintf(gp, "set grid lw 4 \n");
	//fprintf(gp, "set border 15 back lw 3 \n");
	//fprintf(gp, "set xtics 0, 250, 500\n");
	//fprintf(gp, "set ytics 0,250,500 \n");
	fprintf(gp, "set palette maxcolors 2\n");
	fprintf(gp, "set palette defined (0 'white', 1 'black')\n");
	// Setting title
	//fprintf(gp, "set title \"Recurrence Plot Test\" \n");
	//fprintf(gp, "set title \"Test RPs normalized\"\n");
	fprintf(gp, "set output '%s'\n",filename);
	fprintf(gp, "plot 'rp.dat' w p pt 5 ps 0.15 lc 'black' notitle\n");
	//fprintf(gp, "plot 'coord_rp.dat' w p pt 7 ps 0.8 lc 'black' notitle\n");
	//fprintf(gp, "pause -1");
	fclose(gp);
}

void plot_gnuplot_RPs_title(char filename[], int N, int tau, int emb_dim, double eps)
{	
	FILE *gp;
	gp = popen(GNUPLOT, "w");
	fprintf(gp, "set terminal png enhanced crop size 2500,2500 font \"CMR10\" 60 \n");
	fprintf(gp, "set size square \n");
	//fprintf(gp, "unset key \n");
	//fprintf(gp, "unset xtics \n");
	//fprintf(gp, "unset ytics \n");
	fprintf(gp, "unset colorbox \n");
    fprintf(gp, "set yrange[0:%d] \n", N);
    fprintf(gp, "set xrange[0:%d] \n", N);
	//fprintf(gp, "set format x '' \n");
	//fprintf(gp, "set format y '' \n");
	//fprintf(gp, "set grid lw 4 \n");
	//fprintf(gp, "set border 15 back lw 3 \n");
	//fprintf(gp, "set xtics 0, 250, 500\n");
	//fprintf(gp, "set ytics 0,250,500 \n");
	fprintf(gp, "set palette maxcolors 2\n");
	fprintf(gp, "set palette defined (0 'white', 1 'black')\n");
	// Setting title
	fprintf(gp, "set title \"Recurrence Plot Test\\n{/*0.8 Iterations = %d; {/Symbol t} = %d; {/Symbol e} = %1.4f\" \n", N, tau, eps);
	//fprintf(gp, "set title \"Recurrence Plot Test\" \n");
	//fprintf(gp, "set title \"Test RPs normalized\"\n");
	fprintf(gp, "set output '%s'\n",filename);
	fprintf(gp, "plot 'rp.dat' u 1:2 w p pt 5 ps 0.8 lc 'black' noti, 'rp.dat' u 2:1 w p pt 5 ps 0.8 lc 'black' noti \n");
	//fprintf(gp, "plot 'coord_rp.dat' w p pt 7 ps 0.8 lc 'black' notitle\n");
	//fprintf(gp, "pause -1");
	fclose(gp);
}

void plot_gnuplot_bulk_rps(char filename[], int N)
{
    FILE *gp = popen(GNUPLOT, "w");
    if (gp == NULL)
    {
        fprintf(stderr, "Error opening pipe to gnuplot.\n");
        return;
    }
    fprintf(gp, "set terminal png size %d,%d \n", N, N);
    fprintf(gp, "set size square \n");
	fprintf(gp, "unset key \n");
	fprintf(gp, "unset xtics \n");
	fprintf(gp, "unset ytics \n");
	fprintf(gp, "unset cbtics \n");
	fprintf(gp, "unset colorbox \n");
	fprintf(gp, "unset border \n");
	fprintf(gp, "set lmargin 0 \n");
	fprintf(gp, "set rmargin 0 \n");
	fprintf(gp, "set tmargin 0 \n");
	fprintf(gp, "set bmargin 0 \n");
	//fprintf(gp, "set autoscale yfix \n");
	//fprintf(gp, "set autoscale xfix \n");
    fprintf(gp, "set xrange [-0.5:%d.5] \n", N-1);
    fprintf(gp, "set yrange [%d.5:-0.5] \n", N-1);
    fprintf(gp, "set palette maxcolors 2\n");
    fprintf(gp, "set palette defined (0 'white', 1 'black')\n");
    fprintf(gp, "set output '%s'\n", filename);
	//fprintf(gp, "plot 'rp.dat' w p pt 5 ps 0.2 lc 'black' notitle\n");
    fprintf(gp, "plot 'rm.dat' matrix w image\n");
    fclose(gp);
}

void plot_gnuplot_RQA_distributions(char plot_name[], char *name_rqa, double avg, double eps, int size)
{
	FILE *gp;
	gp = popen(GNUPLOT, "w");
	fprintf(gp, "set terminal png enhanced size 2000,1000 font \"../misc/cmr10.ttf\" 40 \n");
	fprintf(gp, "set colors classic \n");

	fprintf(gp, "stats 'results/%d/1st_spec_%s.dat' using 2 name 'y' nooutput \n", size, name_rqa);

	fprintf(gp, "set tics scale 0 \n");
	//fprintf(gp, "set ytics %1.2f \n", avg);

	fprintf(gp, "set autoscale y \n");
	fprintf(gp, "set xrange [0-0.75:200]\n");

	fprintf(gp, "myOffset = %f \n", avg);
	fprintf(gp, "myWidth = 0.5 \n");
	fprintf(gp, "set style fill solid 0.5 border lc 'black' \n");
	
	bool fig_with_parameter_detail = false;
	if (fig_with_parameter_detail)
	{
		fprintf(gp, "set title \"%s distribution \\n{/*0.8 Size = %d; {/Symbol e} = %1.4f\" \n", name_rqa, size, eps);
		fprintf(gp, "set output '%s'\n", plot_name);
	}
	else
	{
		fprintf(gp, "unset key \n");
		fprintf(gp, "unset cbtics \n");
		fprintf(gp, "unset colorbox \n");
		fprintf(gp, "unset border \n");
		fprintf(gp, "set lmargin 0 \n");
		fprintf(gp, "set rmargin 0 \n");
		fprintf(gp, "set tmargin 0 \n");
		fprintf(gp, "set bmargin 0 \n");
		fprintf(gp, "set output '%s_.png'\n", plot_name);
	}

		fprintf(gp, "plot 'results/%d/1st_spec_%s.dat' u 0:2:($0-myWidth):($0+myWidth):(myOffset):2 w boxxyerror noti lc 'black', 'results/%d/1st_spec_%s_avg_upp_low.dat' u 1:2 w l lw 4 lc 'black' noti, 'results/%d/1st_spec_%s_avg_upp_low.dat' u 1:3 w l lw 6 lc 'red' noti, 'results/%d/1st_spec_%s_avg_upp_low.dat' u 1:4 w l lw 6 lc 'red' noti \n", size, name_rqa, size, name_rqa, size, name_rqa, size, name_rqa);

	fclose(gp);
}

void plot_gnuplot_refining_RQA_distributions(char plot_name[], char *name_rqa, double avg, double eps, int size)
{
	FILE *gp;
	gp = popen(GNUPLOT, "w");
	fprintf(gp, "set terminal png enhanced size 2000,1000 font \"../misc/cmr10.ttf\" 40 \n");
	fprintf(gp, "set colors classic \n");

	fprintf(gp, "stats 'results/%d/1st_spec_%s.dat' using 2 name 'y' nooutput \n", size, name_rqa);

	fprintf(gp, "set tics scale 0 \n");
	//fprintf(gp, "set ytics %1.2f \n", avg);

	fprintf(gp, "set yrange [0:6] \n");
	fprintf(gp, "set xrange [0-0.75:200]\n");

	fprintf(gp, "myOffset = %f \n", avg);
	fprintf(gp, "myWidth = 0.5 \n");
	fprintf(gp, "set style fill solid 0.5 border lc 'black' \n");
	
	bool fig_with_parameter_detail = true;
	if (fig_with_parameter_detail)
	{
		fprintf(gp, "set title \"%s distribution \\n{/*0.8 Size = %d; {/Symbol e} = %1.4f\" \n", name_rqa, size, eps);
		fprintf(gp, "set output '%s'\n", plot_name);
	}
	else
	{
		fprintf(gp, "unset key \n");
		fprintf(gp, "unset cbtics \n");
		fprintf(gp, "unset colorbox \n");
		fprintf(gp, "unset border \n");
		fprintf(gp, "set lmargin 0 \n");
		fprintf(gp, "set rmargin 0 \n");
		fprintf(gp, "set tmargin 0 \n");
		fprintf(gp, "set bmargin 0 \n");
		fprintf(gp, "set output '%s_.png'\n", plot_name);
	}

		fprintf(gp, "plot 'results/%d/1st_spec_%s.dat' u 0:2:($0-myWidth):($0+myWidth):(myOffset):2 w boxxyerror noti lc 'black', 'results/%d/1st_spec_%s_avg_upp_low.dat' u 1:2 w l lw 4 lc 'black' noti, 'results/%d/1st_spec_%s_avg_upp_low.dat' u 1:3 w l lw 6 lc 'red' noti, 'results/%d/1st_spec_%s_avg_upp_low.dat' u 1:4 w l lw 6 lc 'red' noti \n", size, name_rqa, size, name_rqa, size, name_rqa, size, name_rqa);
	

	fclose(gp);
}

void plot_gnuplot_RQA_distributions_average(char plot_name[], char *name_rqa, double avg, double percentage, int N)
{
	FILE *gp;
	gp = popen(GNUPLOT, "w");
	fprintf(gp, "set terminal png enhanced size 2000,1000 font \"CMR10\" 40 \n");
	fprintf(gp, "set colors classic \n");

	//fprintf(gp, "set autoscale y \n");
	//fprintf(gp, "set autoscale x\n");

	fprintf(gp, "stats 'results/RR=%1.2f/%s_N=%d.dat' using 2 name 'y' nooutput \n", percentage, name_rqa, N);
	
	// fprintf(gp, "unset key \n");
	// fprintf(gp, "unset cbtics \n");
	// fprintf(gp, "unset colorbox \n");
	// fprintf(gp, "unset border \n");
	// fprintf(gp, "set lmargin 0 \n");
	// fprintf(gp, "set rmargin 0 \n");
	// fprintf(gp, "set tmargin 0 \n");
	// fprintf(gp, "set bmargin 0 \n");

	fprintf(gp, "myOffset = %f \n", avg);
	fprintf(gp, "myWidth = 0.1 \n");

	fprintf(gp, "set yrange [0:180] \n");
	fprintf(gp, "set xrange [0-0.75:10000]\n");

	fprintf(gp, "set output '%s_.png'\n", plot_name);
	
	char *color = "grey30";  
    if (N == 500) color = "light-red";
    else if (N == 1000) color = "web-blue";
    else if (N == 1500) color = "web-green";
    else if (N == 2000) color = "violet";

	fprintf(gp, "set style fill solid 0.25 border lc '%s' \n", color);

	fprintf(gp, "plot 'results/RR=%1.2f/%s_N=%d.dat' u 0:2:($0-myWidth):($0+myWidth):(myOffset):2 w boxxyerror lc '%s' noti, 'results/RR=%1.2f/%s_N=%d_stats.dat' u 1:2 w l lw 5 lc 'grey90' noti, 'results/RR=%1.2f/%s_N=%d_stats.dat' u 1:3 w l lw 5 lc 'grey90' noti, 'results/RR=%1.2f/%s_N=%d_stats.dat' u 1:4 w l lw 5 lc 'grey90' noti\n", percentage, name_rqa, N, color, percentage, name_rqa, N, percentage, name_rqa, N, percentage, name_rqa, N);

	fclose(gp);
}

void plot_gnuplot_phase_space_title(char png_name[], double parameter, int iter)
{	
	FILE *gp;
	gp = popen(GNUPLOT, "w");
	fprintf(gp, "set terminal png enhanced size 2500,2500 font \"CMR10\"  60 \n");
	fprintf(gp, "set colors classic \n");
	fprintf(gp, "set autoscale yfix \n");
	fprintf(gp, "set autoscale xfix \n");

	// fprintf(gp, "unset key \n");
	// fprintf(gp, "unset cbtics \n");
	// fprintf(gp, "unset xtics \n");
	// fprintf(gp, "unset ytics \n");
	// fprintf(gp, "unset colorbox \n");
	// fprintf(gp, "unset border \n");
	// fprintf(gp, "set lmargin 0 \n");
	// fprintf(gp, "set rmargin 0 \n");
	// fprintf(gp, "set tmargin 0 \n");
	// fprintf(gp, "set bmargin 0 \n");

	fprintf(gp, "set title \"Phase Space Test\\n{/*0.7 Control Parameter = %.5f; Max Iterations = %d;}\" \n", parameter, iter); 

	fprintf(gp, "set output '%s'\n", png_name);

	fprintf(gp, "plot 'test_phase_space.dat' u 1:2:0 w p pt 7 ps 3 palette noti,  'initial_conditions.dat' w p pt 7 ps 7 lw 5 lc 'forest-green' noti \n");
	//fprintf(gp, "pause -1");
	fclose(gp);
}
