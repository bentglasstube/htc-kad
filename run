#!/usr/bin/env perl

use strict;
use warnings;

use Math::BigInt;

sub read_args {
  my %args = ();
  while (@ARGV) {
    my ($name, $value) = split /=/, shift @ARGV;
    $name =~ s/^--//;
    $args{$name} = $value;
  }

  return %args;
}

sub read_list {
  my @list = ();
  while (<STDIN>) {
    chomp;
    last unless $_;
    push @list, Math::BigInt->new($_);
  }

  return @list;
}

sub get_nearest {
  my ($key, $list, $count) = @_;

  my @distances = map { [ $_, $key->copy->bxor($_) ] } @$list;

  my @mins = sort { $a->[1] <=> $b->[1] } @distances[0 .. $count - 1];
  shift @distances for 1 .. $count;

  foreach my $candidate (@distances) {
    next if $candidate->[1] >= $mins[-1][1];
    $mins[-1] = $candidate;
    @mins = sort { $a->[1] <=> $b->[1] } @mins;
  }

  return map { $_->[0] } @mins;
}

my %args = read_args;
my @list = read_list;

my $size = $args{request_size} || 1;

while (<STDIN>) {
  chomp;

  my $key = Math::BigInt->new($_);
  my @best = get_nearest($key, \@list, $size);

  print "$_\n" for @best;
}
