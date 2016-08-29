$siteInfo->{'pro-linux'} = {
	'name' => "pro-linux",
	'host' => "pl-forum.de",
	'path' => "/backend/pro-linux.rdf",
	'func' => \&processPoorRdf,
	'flds' => 'title, link, description',
};

$siteInfo->{'ard'} = {
	'name' => "ard",
	'host' => "tagesschau.de",
	'path' => "/newsticker.rdf",
	'func' => \&processPoorRdf,
	'flds' => 'title, link, description',
};

$siteInfo->{'heise'} = {
	'name' => "heise",
	'host' => "www.heise.de",
	'path' => "/newsticker/heise.rdf",
	'func' => \&processPoorRdf,
	'flds' => 'title, link, description',
};

1;

