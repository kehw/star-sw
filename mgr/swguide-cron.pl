#!/opt/star/bin/perl
#
# $Id: swguide-cron.pl,v 1.4 1999/09/20 22:55:17 wenaus Exp $
#
# $Log: swguide-cron.pl,v $
# Revision 1.4  1999/09/20 22:55:17  wenaus
# Move output area to RCF NFS web area
#
# Revision 1.3  1999/07/25 16:27:30  wenaus
# Debug printout only
#
# Revision 1.2  1999/07/22 22:05:05  wenaus
# kill printout
#
# Revision 1.1  1999/07/07 13:21:07  wenaus
# faster and more info presented
#
#
######################################################################
#
# swguide-cron.pl
#
# T. Wenaus 6/99
#
# Build the SW guide static pages for each release
#
# Usage: swguide-cron.pl [dev .dev new pro old]
#

$fpath = "/star/starlib/doc/www/html/comp-nfs";
$pgm = "/star/u2d/wenaus/datadb/swguide.pl";
if ( @ARGV ) {
    @ver = @ARGV;
} else {
    @ver = ( "dev", ".dev", "new", "pro", "old" );
}

$debugOn = 0;
if ( $debugOn ) {
    print "Build SW guide for versions";
    foreach $v ( @ver ) {
        print " '$v'";
    }
    print "\n";
}

@detail = ( 0, 1, 2 );

foreach $v ( @ver ) {
    foreach $d ( @detail ) {
        $fname = $fpath."/swguide-$v-$d.html";
        if ( -e $fname ) { unlink($fname) or die "Can't delete $fname: $!\n"; }
        open(FILE,">$fname") or die "Can't write to $fname: $!\n";
        print "$v-$d\n" if $debugOn;
        $command = "$pgm ver=$v detail=$d dynamic=yes";
        $output = `$command`;
        print FILE $output;
        close (FILE);
    }
}
