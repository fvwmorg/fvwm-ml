#!/usr/bin/perl
# FvwmCommand example - auto raise only selected class
# arg1 - class name in regex
# arg2 - delay (in second) 

if ($#ARGV<0) {
    ($name) = ($0 =~ m@([^/]*?)$@); 
    print STDERR "$name <class> <delay>\n";
    print STDERR "<class> classes to raise in regex\n";
    print STDERR "<delay> to stay in focus to raise\n\n";
    exit(1);
}
$class = shift;
$delay = shift;

$fifo = "$ENV{'HOME'}/.FCsr"; #dedicated fifo set
unlink ("${fifo}C");
unlink ("${fifo}M");
unlink ("${fifo}R");

open( FCM, "FvwmCommand -f $fifo -S $fifo -m -i3 </dev/null |" ) || die "FCM";

while (! -e "${fifo}C") {
    sleep 1;
    print "waiting fifo to be created\n";
}

open( FCC, ">${fifo}C" ) || die "FCC";

select( FCC ); $| = 1;
select( STDOUT ); $| = 1;

print FCC "send_windowlist\n";

$focus_win = '';
$last_win = '';

$rin = '';
vec ($rin, fileno(FCM), 1) = 1;

LOOP1:while(1) {
    $nfound = select ($rout=$rin,undef,undef, 
		      ($focus_win eq $last_win) ? undef : $delay);
    
    if ($nfound > 0) {
	$_ = sysreadline();
	if( /^(0x\S+) class\s+(.*)/ ) {
	    # keep window id
	    $Config{$1} = $2;
	}elsif( /^(0x\S+) focus change/ ) {
	    $focus_win = $1;
	}
    }

    if ((($nfound==0) == ($delay!=0)) &&
	$focus_win ne $last_win) {
	# new message and timeout==0  or */
	# no message and timeout>0 */
	if ($Config{$focus_win} =~ $class) {
	    # raise it
	    print FCC "windowid $focus_win Raise\n";
	}
        $last_win = $focus_win;	# switch to wait mode again */
    }

}

sub sysreadline {
    my $line = '';
    while ($line !~ /\n/) {
	sysread(FCM, $data, 1);
	$line .= $data;
    }
    $line;
}

