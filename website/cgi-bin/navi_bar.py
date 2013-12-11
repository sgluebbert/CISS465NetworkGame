




def navi_bar():
	n= '<style>'
	n+= 'table{ width: 100%; text-align: center;}'
	n+= 'td {width: 50;}'
	n+= '</style>'
	n+=	'<center><h1>Welcome to Star Clash</h1></center>'
	n+= '<table>'
	n+= '<tr>'
	n+='<td><a href="home.py">Sign In</a></td>'
	n+='<td><a href="user_registration.py">Sign Up</a></td>'
	n+='<td>View Lobbies</td>'
	n+='<td><a href="contact.py">Contact Us</a></td>'
	n+='<td>Download</td>'
	n+='<td>Game Play</td>'
	n+='<td><a href="taylor/files/aboutDevelopers.py">About</a></td>'
	n+='</tr>'
	n+='</table><br /><br />'
	return n
