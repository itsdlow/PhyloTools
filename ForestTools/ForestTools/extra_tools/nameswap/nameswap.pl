#!/usr/bin/perl
use strict;
use warnings;

use FileHandle;

package NameSwap;

#static globals
my $ignore_line_regex = qr/^#|^\s*$/;
my $key_delimeter =',';
my $newick_key_regex = qr/[a-zA-Z_]/;
# my $names_input = 'mito_sequences_30_nameswap.txt';
# my $newick_input = 'LargeListTreeMrBayes_30_0.newick';

sub main
{
	print "Hello world\n";

	my($names_input, $newick_input) = @ARGV;

	my $names_dict_ref = create_swap_names_dict($names_input);

	my $newick_output = 'swapped_' . "$newick_input";
	# open newick file -- for WRITING '>'
	my $fh = FileHandle->new("> $newick_output") or die "Couldn't open file $newick_output, $!";
	# read, parse, and duplicate --with swapped names-- newick tree
	print $fh join("\n\n", @{create_name_swapped_newick($newick_input, $names_dict_ref)});
}

sub create_name_swapped_newick
{
	my ($newick_input, $names_dict_ref) = @_;
	my @ret;

	#read in newick tree
	# open post file -- for READ '<'
	my $fh = FileHandle->new("< $newick_input") or die "Couldn't open file $newick_input, $!";
	while ($_ = $fh->getline())
	{
		if($_ =~ /$ignore_line_regex/)
		{
			# body...
			print "NEXT!\n";
			next;
		}
		# print "$_\n";

		#split into array of chars
		my @chars = split('', $_);
		push(@ret, swap_newick_keys(\@chars, $names_dict_ref));
	}
	$fh->close();

	return \@ret;
}

sub swap_newick_keys
{
	my ($chars_ref, $names_dict_ref) = @_;
	# iterate chars of files line AND parse post file
	my $val_started = 0;
	my $ret = "";
	my $key = "";
	# iterate chars of files line AND parse post file
	foreach my $char (@$chars_ref)
	{
		# if char is a letter || _underscore_ [a-zA-Z_]
	    # if char of newick key...
		if($char =~ $newick_key_regex)
		{
			# key char found ==> store in key...
			$key = $key . $char;
			$val_started = 1;
		}
		else
		{

			if($val_started)
			{
				$val_started = 0;
				# store $dict{$val} (use $ val as key)
				my $swapped_key = $names_dict_ref->{$key};
				if($swapped_key)
				{
					# print "$swapped_key\n";
					#append key to newick expression
					$ret = $ret . $swapped_key;
				}
				else
				{
					#$ret = $ret . "UNKNOWN";
					print "swap_newick_keys => ERROR:: KEY '$key' NOT FOUND\n";
				}

					#key stored
				undef($key);
			}
			$ret = $ret . $char;
		}
	}

	return $ret;
}

sub create_swap_names_dict
{
	my $names_input = $_[0];
	# NOTE:: NEWICK EXPRESSION EXCEPTION
		# ===> LEAVE ALL INSTANCES OF e alone... lazy...
	my %names_dict = ('e' => 'e');
	
	# open post file -- for READ '<'
	my $fh = FileHandle->new("< $names_input") or die "Couldn't open file $names_input, $!";
	while ($_ = $fh->getline())
	{
		if($_ =~ /$ignore_line_regex/)
		{
			# body...
			print "NEXT!\n";
			next;
		}
		# print "$_\n";

		#split into array of chars
		my @chars = split('', $_);
		my @dict_entry = extract_key_value_pair(\@chars);
		print "@dict_entry\n";
		$names_dict{"$dict_entry[0]"} = "$dict_entry[1]";
	}
	$fh->close();

	return \%names_dict;
}

# stores key value pair in array -- with arr[0] => key... arr[1]->arr[n] => values
sub extract_key_value_pair
{
	my $chars_ref = $_[0];
	# iterate chars of files line AND parse post file

	my $val_started = 0;
	my $val = "";
	my $key = "";
	# iterate chars of files line AND parse post file
	foreach my $char (@$chars_ref)
	{
	    # not a delimeter pos
		if($key_delimeter ne $char)
		{
			# print "$char";
			$val = $val . $char;
		}
		else
		{
			# key found + stored in $val ==> store in key...
			print "Key: $val\n";
			$key = $val;
			#key stored
			undef($val);
			$val_started = 1;
		}
	}
	if(!$val_started)
	{
		print "VALUE NOT CREATED... key_delimeter: '$key_delimeter', not found\n";
	}
	else
	{
		# trim whitespsace from both key-val strings
			# Applying trim to str1 
		$key =~ s/^\s+|\s+$//g; 
		$val =~ s/^\s+|\s+$//g;
	}
	return ($key, $val);
}


main();