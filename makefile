
push:
	cd client;\
	make clean
	cd server;\
	make clean
	git add *
	git status
	@echo ""
	@read -p "Commit Comment: " MSG;\
	git commit -m "$$MSG"
	git push