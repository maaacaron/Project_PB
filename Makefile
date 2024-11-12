
createSkill: createSkill.o
	gcc -o createSkill createSkill.o
updateSkill: updateSkill.o
	gcc -o updateSkill updateSkill.o
createSkill.o: createSkill.c skill.h
	gcc -c createSkill.c
updateSkill.o: updateSkill.c skill.h
	gcc -c updateSkill.c
clear:
	rm -rf createMoster createMonster.o
