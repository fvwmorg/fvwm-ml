#!/usr/bin/perl
# FvwmTrackFocus
#   -Keep track of the raised window for each page.
#
# Peter McAlpine, Nov 2002
#

use lib `fvwm-perllib dir`;
use FVWM::Module;


######################## sub's ####################

# sigNewPage :: exec'd upon M_NEW_PAGE
sub sigNewPage {
    my ($module, $event) = @_;
    
    my $width = $event->_vp_width;
    my $height = $event->_vp_height;

    $cur_nx = $event->_vp_x / ($width + $psize_x) * (($width + $psize_x) / $psize_x);
    $cur_ny = $event->_vp_y / ($height + $psize_y) * (($height + $psize_y) / $psize_y);
    $cur_desk = $event->_desk;

#    print STDERR "\nwidth##$width.\n";
#    print STDERR "height#$height.\n";
#    print STDERR "_vp_x##".$event->_vp_x.".\n";
#    print STDERR "_vp_y##".$event->_vp_y.".\n";
#    print STDERR "cur_nx#$cur_nx.\n";
#    print STDERR "cur_ny#$cur_ny.\n\n";

    if (!defined($id[$cur_nx][$cur_ny][$cur_desk])) {
#	print STDERR "****caught null****\n";
	$module->send("Next [CurrentPage !iconic] Focus");
    } else {
#	print STDERR "****id[$cur_nx][$cur_ny]=".$id[$cur_nx][$cur_ny]."****\n";
	$module->send(sprintf("WindowId 0x%x Focus NoWarp", $id[$cur_nx][$cur_ny][$cur_desk]));
    }
}

# sigNewDesk :: exec'd upon M_NEW_DESK
sub sigNewDesk {
    my ($module, $event) = @_;

    $cur_desk = $event->_desk;

    if (!defined($id[$cur_nx][$cur_ny][$cur_desk])) {
	$module->send("Next [CurrentPage !iconic] Focus");
    } else {
	$module->send(sprintf("WindowId 0x%x Focus NoWarp", $id[$cur_nx][$cur_ny][$cur_desk]));
    }
}

# sigFocusChange :: exec'd upon M_FOCUS_CHANGE
sub sigFocusChange {
    my ($module, $event) = @_;

    $id[$cur_nx][$cur_ny][$cur_desk] = $event->_win_id;
}


##################### main #########################

# figure out page size aka screen res
open(XWININFO, "xwininfo -root|") || die "can't run xwininfo";
while ( <XWININFO> ) {
    $psize_x = $1 if /Width:\s*(\d+)/;
    $psize_y = $1 if /Height:\s*(\d+)/;
}
close(XWININFO);

# set up module var
my $module = new FVWM::Module(Mask => M_NEW_PAGE | M_FOCUS_CHANGE | M_NEW_DESK,
			      Debug => 0);

$module->addHandler(M_NEW_DESK, \&sigNewDesk);
$module->addHandler(M_NEW_PAGE, \&sigNewPage);
$module->addHandler(M_FOCUS_CHANGE, \&sigFocusChange);

$module->eventLoop;


