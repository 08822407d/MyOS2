#include "include/arch_proto.h"
#include "../../include/proto.h"

void amd64_excep_handler(stack_frame_s * sf)
{
	stack_frame_s *tmp = sf;
	exception_handler(sf->vec_nr);
}