
exe:
	g++ main.cpp

clean:
	rm -f a.out

run:
	./a.out

r:
	./a.out

push:
	make clean
	git add *
	git status
	@echo ""
	@read -p "Commit Comment: " MSG;\
	git commit -m "$$MSG"
	git push