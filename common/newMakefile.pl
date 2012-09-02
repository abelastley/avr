#newMakefile.pl

use strict;
use warnings;

{
	print "NEWMAKEFILE.PL INFO: newMakefile.pl is starting\n";
	my $device;
	my @lines;
	my @bigarray;

	my $numargs = $#ARGV + 1;
	if($numargs == 0) {displayusage();}
	elsif($numargs == 1) {$device = "attiny45";}
	else {$device = $ARGV[1]};

	my $newprojname = $ARGV[0];
	

	open(FILE, "MakefileTemplate") || die "NEWMAKEFILE.PL ERROR: Couldn't open file: $!";
	while(<FILE>) {
		push @lines, $_ ;
	}
	close(FILE);
	
	for(my $i = 0; $i < @lines; $i+=1) {
		my @words = split(/ /, $lines[$i]);
		if($words[0] eq "PROJECT") {
			if($words[1] eq "=") {
				print "old name: $words[2]new name: $newprojname\n";
				$words[2] = $newprojname . "\n";
				$lines[$i] = join(' ', @words);
				print "line number $i in the Makefile was changed to $lines[$i]";
			}
			else {;}
		}
		else {;}		
	}
	
	open(NEWFILE, ">Makefile");
	for(my $i = 0; $i < @lines; $i+=1) {
		print NEWFILE ("$lines[$i]");
	}
	
	close(NEWFILE);
	print "NEWMAKEFILE.PL INFO: newMakefile.pl is finished\n";


}	
	
sub displayusage {

die "
***********************************************************************
********                                                       ********
********              newMakefile.pl usage                     ********
********                                                       ********
***********************************************************************

newMakefile.pl will create a new makefile, called \"Makefile\" based
on a template called \"MakefileTemplate\" which must be present in the
same directory as newMakefile.pl.

******** general usage is: ********

perl newMakefile.pl PROJECT_NAME [DEVICE_NAME]


******** examples: ********

perl newMakefile.pl myawesomeproject
perl newMakefile.pl myawesomeproject attiny13a

******** notes: ******** 

PROJECT_NAME can be pretty much anything.  DEVICE_NAME should be the
name of an AVR device.

";

}





