/**
 * @file   list.h
 * @author Muhammad Khubaib Umer
 * @date   25 October 2019
 * @version 1.0
 * @brief   Macro Based Doubly-Linked List (Stack)
 */

#ifndef __CONTAINER_H__
#define __CONTAINER_H__


#ifndef __KERNEL__	///< Make Compatible with LKM
 #define kmalloc(x, y) calloc(1, x)
 #define kfree(x) free(x)
 #ifndef EXPORT_SYMBOL
  #define EXPORT_SYMBOL(x)
 #endif	/*	EXPORT_SYMBOL	*/
#endif	/*	__KERNEL__	*/

/*		Generic Link-list to store credentials	*/
typedef struct data_node {
	void *data;
	struct data_node *prev, *next;
} data_node_t;

#define LIST_CAST(x) ((data_node_t *) x)

/*	Check if node is empty	*/
#define is_empty_node(head)												\
	({																	\
		(data_node_t*) (*head) == NULL ? 1 : 0;							\
	})

/*	Find Data in list		*/
#define find_node(head, _data, len)										\
	({																	\
		data_node_t *node = (data_node_t*) *head;						\
		while(node != NULL) {											\
			if(memcmp(_data, node->data, len) == 0)						\
				break;													\
			else  														\
				node = node->next;										\
		}																\
		(node);															\
	})

/*		Find Data in List on some offset		*/
#define find_node_field(_head, _key, _offset, _len)						\
	({																	\
		data_node_t *node = (data_node_t*) *_head;						\
		while(node != NULL) {											\
			if(memcmp(node->data + _offset, _key, _len) == 0)			\
				break;													\
			node = node->next;											\
		}																\
		(node);															\
	})

/*		Insert Data at the top of stack		*/
#define insert_node(head, _data)										\
	({																	\
		data_node_t *node = kmalloc(sizeof(data_node_t), GFP_KERNEL);	\
		node->data = _data;												\
		node->prev = NULL;												\
		node->next = (data_node_t*) (*head);							\
		if(((data_node_t**) *head) != NULL)								\
			(*head)->prev = node;										\
		*head = node;													\
		(*head);														\
	})

/*		Delete Data top of stack		*/
#define delete_node(_head, _node)										\
	({																	\
		do{																\
			data_node_t **head = _head;									\
			data_node_t *node = _node;									\
			if(*(head) == NULL	|| (node) == NULL)						\
				break;													\
			if(*(head) == (node))										\
				*(head) = (node->next);									\
			if((node->next) != NULL)									\
				(node->next->prev) = (node->prev);						\
			if((node->prev) != NULL)									\
				(node->prev->next) = (node->next);						\
			kfree(node);												\
		} while(0);														\
	})

/*		Delete a complete List		*/
#define delete_list(_head)												\
	({																	\
		data_node_t *_head_ = (data_node_t*) *_head;					\
		do{																\
			if(is_empty_node(&_head_))									\
				break;													\
			;;;															\
			while(!is_empty_node(&_head_)) {							\
				delete_node(&_head_, _head_);							\
			}															\
		} while(0);														\
	})

#define get_next_node(_node)											\
	({																	\
		data_node_t *_node_ = (data_node_t*) *_node;					\
		do {															\
			if(_node_ == NULL)											\
				break;													\
			;;;															\
			if(_node_->next == NULL)									\
				break;													\
			;;;															\
			_node_ = _node_->next;										\
		} while(0);														\
		(_node_);														\
	})

/*		foreach_node_callback register		*/
/*		void (*callback_f)(data_node_t*)	*/
#define foreach_node_callback(_head, FUNC)								\
	({																	\
		data_node_t *node = (data_node_t*) *_head;						\
		do{																\
			if(is_empty_node(&node))									\
				break;													\
			;;;															\
			while(node != NULL)	{										\
				FUNC(node);												\
				node = node->next;										\
			}															\
		} while(0);														\
	})

#define foreach_node_free(_head)										\
	({																	\
		data_node_t *node = (data_node_t*) *_head;						\
		do {															\
			if(is_empty_node(&node))									\
				break;													\
			;;;															\
			data_node_t *tmp = node;									\
			while(node != NULL) {										\
				tmp = node;										\
				node = node->next;										\
				free(tmp);												\
			}															\
		} while(0);														\
	})

#endif /*	__CONTAINER_H__	*/
