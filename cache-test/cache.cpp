#include<stdio.h>
#include<ccontrol.h>
void do_stuff(char *t, size_t s);
int main(void) {
	char *t;
	struct ccontrol_zone *z;
	color_set c;

	COLOR_ZERO(&c);
	for(int i=0; i< 32; i++)
		COLOR_SET(i,&c);

	z= ccontrol_new();
	ccontrol_create_zone(z,&c,400);

	t = (char *) ccontrol_malloc(z,100);
	do_stuff(t,100);

	ccontrol_free(z,t);
	ccontrol_destroy_zone(z);
	ccontrol_delete(z);
	return 0;
}
