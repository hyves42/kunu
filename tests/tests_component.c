#include "tests_component.h"
#include "kn_component.h"
#include <check.h>
#include <stdio.h>
#include <stdbool.h>

// test if all members are initialized correctly
START_TEST(component_tests_init)
{
	kn_event_t buf[FIFO_SIZE_32];
	kn_event_reg_t disp_map[]={
		KN_EVENT_REGISTER(1, NULL, NULL),
		KN_EVENT_REGISTER(2, NULL, NULL),
		KN_EVENT_REGISTER(3, NULL, NULL),
	};
	kn_event_reg_t out_disp_map[10]={};
	kn_component_t component=KN_COMPONENT_INIT(component, buf, disp_map, out_disp_map);


	ck_assert(component.worker_if.user_data==&component);
	ck_assert(component.schedulable_if.user_data==&component);
	// check callbacks
	ck_assert(component.worker_if.on_event==kn_component_on_event);
	ck_assert(component.schedulable_if.schedule==kn_component_schedule);

	ck_assert(component.events_fifo.buf==buf);
	ck_assert(component.events_fifo.size==FIFO_SIZE_32);

	ck_assert(component.input_disp.map_size==3);

}
END_TEST

int tests_component_run(void){
	Suite *s1 = suite_create("Component");
	TCase *tc1_1 = tcase_create("Component");
	SRunner *sr = srunner_create(s1);
	int nf;

	suite_add_tcase(s1, tc1_1);
	tcase_add_test(tc1_1, component_tests_init);

	srunner_run_all(sr, CK_ENV);
	nf = srunner_ntests_failed(sr);
	srunner_free(sr);

	return nf == 0 ? 0 : 1;
}