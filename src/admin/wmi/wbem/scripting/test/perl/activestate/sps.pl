
# list all services

use strict;

use Win32::OLE;
use Win32::OLE::Enum;
use Win32::OLE::Const 'Microsoft WMI Scripting V1.1 Library';

use Getopt::Std;

# below is the old version if you should need it.
# use Win32::OLE::Const 'Microsoft WBEM Scripting V1.0 Library';

use vars qw($locator $serv $opt_h $objs);

((2 == @ARGV) || (0 == @ARGV)) || die "Useage: [-h host] \n";
getopts('h:') || die "Useage: [-h host] \n";

$locator = Win32::OLE->new('WbemScripting.SWbemLocator') or die "loc\n";

$serv = $locator->ConnectServer(($opt_h) ? $opt_h : '.');
$serv->{security_}->{ImpersonationLevel} = wbemImpersonationLevelImpersonate;

$objs = $serv->InstancesOf("Win32_service");

print "Started\tStartMode\tStartName\tState\tName\n";
foreach (Win32::OLE::Enum->new($objs)->All())
{
	print "$_->{Started}\t$_->{StartMode}\t\t$_->{StartName}\t$_->{State}\t($_->{Name})\n";
}

