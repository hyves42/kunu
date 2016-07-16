/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

#include "tests_events_dispatch.h"
#include "kn_events_dispatch.h"
#include <check.h>
#include <stdio.h>
#include <stdbool.h>


static bool handler1_called=false;
static bool handler2_called=false;
static bool handler3_called=false;
static bool handler4_called=false;
static bool handler5_called=false;
static int handler1_count=0;
static int handler2_count=0;
static int handler3_count=0;
static int handler4_count=0;
static int handler5_count=0;

static void reset_states(void){
	handler1_called=false;
	handler2_called=false;
	handler3_called=false;
	handler4_called=false;
	handler5_called=false;

	handler1_count=0;
	handler2_count=0;
	handler3_count=0;
	handler4_count=0;
	handler5_count=0;
}

static int event_handler1(int evt_id, void *evt_data, void *user_data){
	ck_assert(evt_id==1);
	handler1_called=true;
	handler1_count++;
	return 0;
}
static int event_handler2(int evt_id, void *evt_data, void *user_data){
	ck_assert(evt_id==2);
	handler2_called=true;
	handler2_count++;
	return 0;
}
static int event_handler3(int evt_id, void *evt_data, void *user_data){
	ck_assert(evt_id==3);
	handler3_called=true;
	handler3_count++;
	return 0;
}
static int event_handler4(int evt_id, void *evt_data, void *user_data){
	ck_assert(evt_id==4);
	handler4_called=true;
	handler4_count++;
	return 0;
}
static int event_handler5(int evt_id, void *evt_data, void *user_data){
	ck_assert(evt_id==5);
	handler5_called=true;
	handler5_count++;
	return 0;
}

static int dummy_handler(int evt_id, void *evt_data, void *user_data){
	return 0;
}

START_TEST(events_tests_init)
{
	kn_event_reg_entry_t map[10]={};
	int map_count=10;
	int ret=kn_events_init(map, map_count);

	ck_assert(ret==0);
}
END_TEST

START_TEST(events_tests_init2)
{
	kn_event_reg_entry_t map[10]={};
	int map_count=10;

	kn_event_reg_t reg={
		.id=1,
		.user_data=NULL,
		.handler=dummy_handler
	};

	int ret=kn_events_init(map, map_count);
	ck_assert(ret==0);

	ret=kn_events_register_subscriber(&reg);
	ck_assert(ret==0);
	ck_assert(kn_events_get_registered_count()==1);

	ck_assert(map[0].id==reg.id);
	ck_assert(map[0].reg==&reg);
}
END_TEST

START_TEST(events_tests_init3)
{
	kn_event_reg_entry_t map[2]={};
	int map_count=2;

	kn_event_reg_t reg1={
		.id=1,
		.user_data=NULL,
		.handler=dummy_handler
	};
	kn_event_reg_t reg2={
		.id=2,
		.user_data=NULL,
		.handler=dummy_handler
	};

	int ret=kn_events_init(map, map_count);
	ck_assert(ret==0);

	ret=kn_events_register_subscriber(&reg1);
	ck_assert(ret==0);
	ck_assert(kn_events_get_registered_count()==1);

	ret=kn_events_register_subscriber(&reg2);
	ck_assert(ret==0);
	ck_assert(kn_events_get_registered_count()==2);	

	ck_assert(map[0].id==reg1.id);
	ck_assert(map[0].reg==&reg1);

	ck_assert(map[1].id==reg2.id);
	ck_assert(map[1].reg==&reg2);	
}
END_TEST

//Check that we can't register more eventss than the size of the registered events array
START_TEST(events_tests_init4)
{
	kn_event_reg_entry_t map[1]={};
	int map_count=1;

	kn_event_reg_t reg={
		.id=1,
		.user_data=NULL,
		.handler=dummy_handler
	};

	int ret=kn_events_init(map, map_count);
	ck_assert(ret==0);

	ret=kn_events_register_subscriber(&reg);
	ck_assert(ret==0);
	ck_assert(kn_events_get_registered_count()==1);

	ret=kn_events_register_subscriber(&reg);
	ck_assert(ret==-1);
	ck_assert(kn_events_get_registered_count()==1);	
}
END_TEST

//Check ordering
START_TEST(events_tests_init5)
{
	kn_event_reg_entry_t map[6]={};
	int map_count=6;

	kn_event_reg_t reg1={
		.id=1,
		.user_data=NULL,
		.handler=dummy_handler
	};
	kn_event_reg_t reg2={
		.id=2,
		.user_data=NULL,
		.handler=dummy_handler
	};
	kn_event_reg_t reg3={
		.id=3,
		.user_data=NULL,
		.handler=dummy_handler
	};
	kn_event_reg_t reg4={
		.id=4,
		.user_data=NULL,
		.handler=dummy_handler
	};
	kn_event_reg_t reg5={
		.id=5,
		.user_data=NULL,
		.handler=dummy_handler
	};

	int ret=kn_events_init(map, map_count);
	ck_assert(ret==0);

	ret=kn_events_register_subscriber(&reg1);
	ck_assert(ret==0);
	ck_assert(kn_events_get_registered_count()==1);

	ret=kn_events_register_subscriber(&reg5);
	ck_assert(ret==0);
	ck_assert(kn_events_get_registered_count()==2);

	ret=kn_events_register_subscriber(&reg2);
	ck_assert(ret==0);
	ck_assert(kn_events_get_registered_count()==3);	

	ret=kn_events_register_subscriber(&reg4);
	ck_assert(ret==0);
	ck_assert(kn_events_get_registered_count()==4);

	ret=kn_events_register_subscriber(&reg3);
	ck_assert(ret==0);
	ck_assert(kn_events_get_registered_count()==5);

	ck_assert(map[0].id==reg1.id);
	ck_assert(map[0].reg==&reg1);

	ck_assert(map[1].id==reg2.id);
	ck_assert(map[1].reg==&reg2);	

	ck_assert(map[2].id==reg3.id);
	ck_assert(map[2].reg==&reg3);	

	ck_assert(map[3].id==reg4.id);
	ck_assert(map[3].reg==&reg4);	

	ck_assert(map[4].id==reg5.id);
	ck_assert(map[4].reg==&reg5);	
	

}
END_TEST



//Check ordering
START_TEST(events_tests_init6)
{
	kn_event_reg_entry_t map[6]={};
	int map_count=6;

	kn_event_reg_t reg1={
		.id=1,
		.user_data=NULL,
		.handler=dummy_handler
	};
	kn_event_reg_t reg2={
		.id=2,
		.user_data=NULL,
		.handler=dummy_handler
	};
	kn_event_reg_t reg3={
		.id=3,
		.user_data=NULL,
		.handler=dummy_handler
	};
	kn_event_reg_t reg4={
		.id=3,
		.user_data=NULL,
		.handler=dummy_handler
	};
	kn_event_reg_t reg5={
		.id=5,
		.user_data=NULL,
		.handler=dummy_handler
	};

	int ret=kn_events_init(map, map_count);
	ck_assert(ret==0);

	ret=kn_events_register_subscriber(&reg1);
	ck_assert(ret==0);
	ck_assert(kn_events_get_registered_count()==1);

	ret=kn_events_register_subscriber(&reg5);
	ck_assert(ret==0);
	ck_assert(kn_events_get_registered_count()==2);

	ret=kn_events_register_subscriber(&reg2);
	ck_assert(ret==0);
	ck_assert(kn_events_get_registered_count()==3);	

	ret=kn_events_register_subscriber(&reg4);
	ck_assert(ret==0);
	ck_assert(kn_events_get_registered_count()==4);

	ret=kn_events_register_subscriber(&reg3);
	ck_assert(ret==0);
	ck_assert(kn_events_get_registered_count()==5);

	ck_assert(map[0].id==reg1.id);
	ck_assert(map[0].reg==&reg1);

	ck_assert(map[1].id==reg2.id);
	ck_assert(map[1].reg==&reg2);	

	ck_assert((map[2].id==reg3.id) || (map[2].id==reg4.id));
	ck_assert((map[2].reg==&reg3) || (map[2].reg==&reg4));

	ck_assert((map[3].id==reg3.id) || (map[3].id==reg4.id));
	ck_assert((map[3].reg==&reg3) || (map[3].reg==&reg4));	

	ck_assert(map[4].id==reg5.id);
	ck_assert(map[4].reg==&reg5);
}
END_TEST


//Check ordering
START_TEST(events_tests_array)
{
	kn_event_reg_entry_t map[6]={};
	int map_count=6;

	kn_event_reg_t array[]={{
		.id=1,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=2,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=3,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=4,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=5,
		.user_data=NULL,
		.handler=dummy_handler
	}};

	int ret=kn_events_init(map, map_count);
	ck_assert(ret==0);

	ret=kn_events_register_subscriber_array(array, sizeof(array)/sizeof(array[0]));
	ck_assert(ret==0);
	ck_assert(kn_events_get_registered_count()==5);

	ck_assert(map[0].id==array[0].id);
	ck_assert(map[0].reg==&array[0]);
	ck_assert(map[1].id==array[1].id);
	ck_assert(map[1].reg==&array[1]);	
	ck_assert(map[2].id==array[2].id);
	ck_assert(map[2].reg==&array[2]);	
	ck_assert(map[3].id==array[3].id);
	ck_assert(map[3].reg==&array[3]);	
	ck_assert(map[4].id==array[4].id);
	ck_assert(map[4].reg==&array[4]);	
	
}
END_TEST

//Test failure when we try to register too big an array
START_TEST(events_tests_array2)
{
	kn_event_reg_entry_t map[3]={};
	int map_count=3;

	kn_event_reg_t array[]={{
		.id=1,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=2,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=3,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=4,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=5,
		.user_data=NULL,
		.handler=dummy_handler
	}};

	int ret=kn_events_init(map, map_count);
	ck_assert(ret==0);

	ret=kn_events_register_subscriber_array(array, sizeof(array)/sizeof(array[0]));
	ck_assert(ret<0);
	ck_assert(kn_events_get_registered_count()==3);	
}
END_TEST


//Check ordering
START_TEST(events_tests_broadcast)
{
	kn_event_reg_entry_t map[6]={};
	int map_count=6;
	bool handler_called=false;

	int handler3(int evt_id, void *evt_data, void *user_data){
		ck_assert(evt_id==3);
		handler_called=true;
		return 0;
	}


	kn_event_reg_t array[]={{
		.id=1,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=2,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=3,
		.user_data=NULL,
		.handler=handler3
	},{
		.id=4,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=5,
		.user_data=NULL,
		.handler=dummy_handler
	}};

	int ret=kn_events_init(map, map_count);
	ck_assert(ret==0);

	ret=kn_events_register_subscriber_array(array, sizeof(array)/sizeof(array[0]));
	ck_assert(ret==0);
	ck_assert(kn_events_get_registered_count()==5);

	ret=kn_events_broadcast(3, NULL);
	ck_assert(ret==0);
	ck_assert(handler_called);
}
END_TEST





//Check ordering
START_TEST(events_tests_broadcast2)
{
	kn_event_reg_entry_t map[6]={};
	int map_count=6;


	kn_event_reg_t array[]={{
		.id=1,
		.user_data=NULL,
		.handler=event_handler1
	},{
		.id=2,
		.user_data=NULL,
		.handler=event_handler2
	},{
		.id=3,
		.user_data=NULL,
		.handler=event_handler3
	},{
		.id=4,
		.user_data=NULL,
		.handler=event_handler4
	},{
		.id=5,
		.user_data=NULL,
		.handler=event_handler5
	}};

	int ret=kn_events_init(map, map_count);
	ck_assert(ret==0);

	reset_states();

	ret=kn_events_register_subscriber_array(array, sizeof(array)/sizeof(array[0]));
	ck_assert(ret==0);
	ck_assert(kn_events_get_registered_count()==5);

	ret=kn_events_broadcast(1, NULL);
	ck_assert(ret==0);
	ret=kn_events_broadcast(2, NULL);
	ck_assert(ret==0);
	ret=kn_events_broadcast(3, NULL);
	ck_assert(ret==0);
	ret=kn_events_broadcast(4, NULL);
	ck_assert(ret==0);
	ret=kn_events_broadcast(5, NULL);
	ck_assert(ret==0);

	ck_assert(handler1_count==1);
	ck_assert(handler2_count==1);
	ck_assert(handler3_count==1);
	ck_assert(handler4_count==1);
	ck_assert(handler5_count==1);

}
END_TEST


//Check ordering
START_TEST(events_tests_broadcast3)
{
	kn_event_reg_t array[]={{
		.id=1,
		.user_data=NULL,
		.handler=event_handler1
	},{
		.id=2,
		.user_data=NULL,
		.handler=event_handler2
	},{
		.id=2,
		.user_data=NULL,
		.handler=event_handler2
	},{
		.id=3,
		.user_data=NULL,
		.handler=event_handler3
	},{
		.id=4,
		.user_data=NULL,
		.handler=event_handler4
	},{
		.id=4,
		.user_data=NULL,
		.handler=event_handler4
	},{
		.id=4,
		.user_data=NULL,
		.handler=event_handler4
	},{		
		.id=5,
		.user_data=NULL,
		.handler=event_handler5
	}};

	kn_event_reg_entry_t map[sizeof(array)/sizeof(array[0])]={};
	int map_count=sizeof(array)/sizeof(array[0]);

	int ret=kn_events_init(map, map_count);
	ck_assert(ret==0);

	reset_states();

	ret=kn_events_register_subscriber_array(array, sizeof(array)/sizeof(array[0]));
	ck_assert(ret==0);
	ck_assert(kn_events_get_registered_count()==sizeof(array)/sizeof(array[0]));

	ret=kn_events_broadcast(1, NULL);
	ck_assert(ret==0);
	ret=kn_events_broadcast(2, NULL);
	ck_assert(ret==0);
	ret=kn_events_broadcast(3, NULL);
	ck_assert(ret==0);
	ret=kn_events_broadcast(4, NULL);
	ck_assert(ret==0);
	ret=kn_events_broadcast(5, NULL);
	ck_assert(ret==0);

	ck_assert(handler1_count==1);
	ck_assert(handler2_count==2);
	ck_assert(handler3_count==1);
	ck_assert(handler4_count==3);
	ck_assert(handler5_count==1);

}
END_TEST


//Check ordering
START_TEST(events_tests_broadcast4)
{
	kn_event_reg_t array[]={{
		.id=1,
		.user_data=NULL,
		.handler=event_handler1
	},{
		.id=2,
		.user_data=NULL,
		.handler=event_handler2
	},{
		.id=2,
		.user_data=NULL,
		.handler=event_handler2
	},{
		.id=3,
		.user_data=NULL,
		.handler=event_handler3
	},{
		.id=4,
		.user_data=NULL,
		.handler=event_handler4
	},{
		.id=4,
		.user_data=NULL,
		.handler=event_handler4
	},{
		.id=4,
		.user_data=NULL,
		.handler=event_handler4
	},{		
		.id=5,
		.user_data=NULL,
		.handler=event_handler5
	}};

	kn_event_reg_entry_t map[sizeof(array)/sizeof(array[0])]={};
	int map_count=sizeof(array)/sizeof(array[0]);

	int ret=kn_events_init(map, map_count);
	ck_assert(ret==0);

	reset_states();

	ret=kn_events_register_subscriber_array(array, sizeof(array)/sizeof(array[0]));
	ck_assert(ret==0);
	ck_assert(kn_events_get_registered_count()==sizeof(array)/sizeof(array[0]));

	ret=kn_events_broadcast(6, NULL);
	ck_assert(ret==0);

	ck_assert(handler1_count==0);
	ck_assert(handler2_count==0);
	ck_assert(handler3_count==0);
	ck_assert(handler4_count==0);
	ck_assert(handler5_count==0);

}
END_TEST

int tests_events_dispatch_run(void){
	Suite *s1 = suite_create("Events");
	TCase *tc1_1 = tcase_create("Events");
	SRunner *sr = srunner_create(s1);
	int nf;

	suite_add_tcase(s1, tc1_1);
	tcase_add_test(tc1_1, events_tests_init);
	tcase_add_test(tc1_1, events_tests_init2);
	tcase_add_test(tc1_1, events_tests_init3);
	tcase_add_test(tc1_1, events_tests_init4);
	tcase_add_test(tc1_1, events_tests_init5);
	tcase_add_test(tc1_1, events_tests_init6);
	tcase_add_test(tc1_1, events_tests_array);
	tcase_add_test(tc1_1, events_tests_array2);
	tcase_add_test(tc1_1, events_tests_broadcast);
	tcase_add_test(tc1_1, events_tests_broadcast2);
	tcase_add_test(tc1_1, events_tests_broadcast3);
	tcase_add_test(tc1_1, events_tests_broadcast4);


	srunner_run_all(sr, CK_ENV);
	nf = srunner_ntests_failed(sr);
	srunner_free(sr);

	return nf == 0 ? 0 : 1;
}