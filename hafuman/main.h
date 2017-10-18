
#include<malloc.h>
#include<stdlib.h>


struct request_floor
{
	int floor;
	bool busy;
	struct request_floor * next;
};

struct request_floor *request_down_head , *request_down_tail;


struct request_floor *request_up_head , *request_up_tail;

struct request_floor *target_head , *target_tail;

void AddRequestUpFloor(int r)  //����
{ 
	struct request_floor * p , *buf;
	if(request_up_head == NULL)//��һ��������㣬�½�����
	{
		request_up_head = (struct request_floor *)malloc(sizeof(struct request_floor));
		request_up_tail = request_up_head;
		//request_up_head->next = request_up_tail;
		request_up_head->floor = r;
		request_up_head->next = NULL;
		request_up_tail->next = NULL;

	}
	else 
	{
		if(request_up_head->floor > r)
		{
			p = (struct request_floor *)malloc(sizeof(struct request_floor));
			p->next = request_up_head;
			request_up_head = p;
			request_up_head->floor = r;

		}
		else if(request_up_tail->floor < r)
		{

			p = (struct request_floor *)malloc(sizeof(struct request_floor));
			if(request_up_head == request_up_tail)
			{
				request_up_head->next = p;
				request_up_tail = p;
				request_up_tail->floor = r;
				request_up_tail->next = NULL;
			}else
			{
				request_up_tail->next = p;
				request_up_tail = p;
				request_up_tail->floor = r;
				request_up_tail->next = NULL;
			}

		}
		else
		{
			for(p = request_up_head ; p != request_up_tail ; p = p->next)
			{
				if(p->next->floor > r)
				{
					buf = (struct request_floor *)malloc(sizeof(struct request_floor));
					buf->next = p->next;
					p->next = buf;
					buf->floor =r;
					break;
				}
				if(p->next->floor == r)
				{
					break;
				}
			}
		}

	}
}
void AddRequestDownFloor(int r)       //����
{
	struct request_floor * p , *buf;
	if(request_down_head == NULL)//��һ��������㣬�½�����
	{
		request_down_head = (struct request_floor *)malloc(sizeof(struct request_floor));
		request_down_tail = request_down_head;
		request_down_head->floor = r;
		request_down_head->next = NULL;
		request_down_tail->next = NULL;

	}
	else
	{
		if(request_down_head->floor < r)   //����¥�����ķŵ�����ͷ��
		{
			p = (struct request_floor *)malloc(sizeof(struct request_floor));
			p->next = request_down_head;
			request_down_head = p;
			request_down_head->floor = r;


		}

		else if(request_down_tail->floor > r)   //����¥����С�ķŵ�����β��
		{
			p = (struct request_floor *)malloc(sizeof(struct request_floor));
			if(request_down_head == request_down_tail)
			{
				request_down_head->next = p;
				request_down_tail = p;
				request_down_tail->floor = r;
				request_up_tail->next = NULL;
			}else
			{

				request_down_tail->next = p;
				request_down_tail = p;
				request_down_tail->floor = r;
				request_down_tail->next = NULL;
			}

		}
		else
		{
			for(p = request_down_head ; p != request_down_tail; p = p->next)
			{
				if(p->next->floor < r) //������¥��ŵ����ʵ�λ��ʹ�䱣������
				{
					buf = (struct request_floor *)malloc(sizeof(struct request_floor));
					buf->next = p->next;
					p->next = buf;
					buf->floor = r;
					break;
				}
				if(p->next->floor == r)
				{
					break;
				}


			}

		}
	}
}

void FreeDownRequest(struct request_floor * p)
{
	struct request_floor * buf;
	if(request_down_head == p)
	{
		if(request_down_tail ==p)
		{

			request_down_head = NULL;
			request_down_tail = NULL;
			free(p);
		}else
		{
			request_down_head = request_down_head->next;
			free(p);
		}
	}
	else 
	{
		for(buf = request_down_head ; buf != request_down_tail ; buf = buf->next)
		{
			if(buf->next == p)
			{
				struct request_floor * x = buf->next;
				buf->next = buf->next->next;
				free(p);
				break;
			}
		}
	}


}
void FreeUpRequest(struct request_floor * p)
{
	struct request_floor * buf;

	if(request_up_head == p)
	{
		if(request_up_tail ==p)
		{
			request_up_head = NULL;
			request_up_tail = NULL;
			free(p);
		}else
		{
			request_up_head = request_up_head->next;
			free(p);
		}
	}
	else 
	{
		for(buf = request_up_head ; buf != request_up_tail ; buf = buf->next)
		{
			if(buf->next == p)
			{
				struct request_floor * x = buf->next;
				buf->next = buf->next->next;
				free(p);
				break;
			}
		}
	}
}

void FreeTarget(struct request_floor * p)
{

	struct request_floor * buf;
	if(target_head == p)
	{
		if(target_tail == p)
		{
			target_head = NULL;
			target_tail = NULL;
			free(p);
		}else
		{
			target_head = target_head->next;
		}
	}
	else
	{
		for(buf = target_head ; buf != target_tail ; buf = buf->next)
		{
			if(buf->next == p)
			{
				struct request_floor * x = buf->next;
				buf->next = buf->next->next;
				free(p);
				break;
			}
		}	
	}
}
void AddTargetFloor(int t)     //��������
{
	struct request_floor * p , *buf;
	if(target_head == NULL)   //��һ��Ԫ��
	{

		target_head = (struct request_floor *)malloc(sizeof(struct request_floor));
		target_tail = target_head;
		target_head->floor = t;
		target_head->next = NULL;
		target_tail->next = NULL;
	}
	else
	{
		p = (struct request_floor *)malloc(sizeof(struct request_floor));
		target_tail->next = p;
		target_tail = p;
		target_tail->floor = t;
		target_tail->next = NULL;

	}
}

bool FindRequestUp(int f)//�Ƿ񵽴�ĳ�����������
{
	struct request_floor * buf;
	if(request_up_head != NULL)
	{
		if(request_up_head->floor == f)
		{
			FreeUpRequest(request_up_head);
			return true;
		}
		else
		{
			for(buf = request_up_head ; buf != request_up_tail ; buf = buf->next)
			{	
			if(buf->next->floor == f)
			{
				FreeUpRequest(buf->next);
				return true;
				break;
			}
			}
		}
	}
	return false;
}

bool FindRequestDown(int f)//�Ƿ񵽴�ĳ�����������   ///BUG  ��Ҫ��  �������д������S
{
	struct request_floor * buf;
	if(request_down_head != NULL)
	{
		if(request_down_head->floor == f)
		{
			FreeDownRequest(request_down_head);
			return true;
		}
		else
		{
			for(buf = request_down_head ; buf != request_down_tail ; buf = buf->next)
			{	
				if(buf->next->floor == f)
				{
					FreeDownRequest(buf);
					return true;
					break;
				}
			}
		}
	}
	return false;
}

bool FindTarget(int f)
{
	struct request_floor * buf;

	for(buf = target_head ; buf != NULL ; buf = buf->next)
	{	
		if(buf->floor == f)
		{
			FreeTarget(target_head);
			return true;
			break;
		}
	}
	return false;
}
void ClearRequestUp()
{
	struct request_floor * p;


	for(p = request_up_head ; p != NULL ; p = p->next)
	{
		free(p);

	}
	request_up_head = NULL;
	request_up_tail = NULL;

}

void ClearRequestDown()
{
	struct request_floor * p;

	for(p = request_down_head ; p != NULL ; p = p->next)
	{

		free(p);

	}
	request_down_head = NULL;
	request_down_tail = NULL;

}
void ClearTarget()
{
	struct request_floor * p;
	if(target_head != NULL)
	{
		for(p = target_head ; p != target_tail ; p = p->next)
		{
			free(p);
		}
	}
}