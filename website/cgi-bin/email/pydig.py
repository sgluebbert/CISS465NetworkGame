from subprocess import Popen, PIPE



def dig(s='smtp.gmail.com', verbose=False):
	import os
	p = Popen("dig smtp.gmail.com", shell=True, stdout=PIPE, close_fds=True, bufsize=4096, stderr=PIPE, stdin=PIPE)
	a, b, c = (p.stdin, p.stdout, p.stderr)
	s = b.read()
	if verbose: print s
	s = s.split(';; ANSWER SECTION:')[-1]
	s = s.split(';;')[0]
	i = s.find('\n')
	s = s[i:]
	import re
	p = re.compile('\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}')
	xs = p.findall(s)
	if verbose: print xs
	return xs

if __name__ == '__main__':
    dig('imap.gmail.com', verbose=True)
    
