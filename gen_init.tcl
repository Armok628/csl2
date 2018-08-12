#!/usr/bin/tclsh
proc puts_init_lines {file} {
	if [catch {set fh [open $file r]} err] {
		puts "/*$err*/"
		return
	}
	set lines [split [read $fh] "\n"]
	close $fh
	foreach line $lines {
		if {[regexp {^CORE\(([^,]+),([^,]+),} $line -> name func]} {
			puts "\tinsert(dict,\"$name\",incr_refs(new_cfunction(&stack_$func)));"
		}
	}
}

puts \
"#ifndef INIT
#define INIT"
foreach file $argv {puts "#include \"$file\""}
puts \
"void init_dict(void)
{
	dict=new_namespace();
	push_namespace(dict);
	hash_function=&nocase_hash_key;
	obj_t *f=NULL;"
foreach file $argv {puts_init_lines $file}
puts \
"}
#endif"
