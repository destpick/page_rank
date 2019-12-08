#ifndef PAGE_H
#define PAGE_H

#include <string>
#include <iostream>

///////////////////ADD GETTER AND SETTER FOR PORTS
class Page
{
	private:
		int * outgoing_links;
		int ports;
		int page_id;
		int times_visited;
		long double page_rank;
	
	public:
		////////////////////////////////////////// CONSTRUCTORS //////////////////////
		//Empty
		Page()
		{
			this->outgoing_links = 0;
			this->ports = 0;
			this->page_id = -1;
			this->times_visited = 0;
			this->page_rank = 0;
		}
		
		// Adding a page that has an id already
		Page(int id)
		{
			this->outgoing_links = 0;
			this->ports = 0;
			this->page_id = id;
			this->times_visited = 0;
			this->page_rank = 0;
		}

		////////////////////////// APPROPRIATE GETTERS AND SETTERS ///////////////////////

		int get_page_id()
		{
			return this->page_id;
		}

		void set_page_id(int id)
		{
			this->page_id = id;
		}

		void set_page_rank(int total_visits)
		{
			this->page_rank = (long double)this->times_visited / total_visits;
		}

		long double get_page_rank()
		{
			return this->page_rank;
		}
		int get_time_visited()
		{
			return this->times_visited;
		}

		void increase_time_visited()
		{
			this->times_visited++;
		}

		void set_time_visited(int num)
		{
			this->times_visited = num;
		}

		void increment_port()
		{
			this->ports++;
		}

		int get_ports()
		{
			return this->ports;
		}

		int get_outgoing_link_id(int index)
		{
			return this->outgoing_links[index];
		}

		/*
			Finding the link at a specific position
			Return:
				index if exisit
				-1 if index does not exist
		*/
		int get_outgoing_link_position(int page)
		{
			if (this->outgoing_links[page])
			{
				return this->outgoing_links[page];
			}

			else
			{
				return -1;
			}
		}
		int link_already_exist(int id)
		{
			for (int i = 0; i < this->ports; i++)
			{
				if (id == this->outgoing_links[i])
				{
					return 1;
				}
			}
			return 0;
		}
		void set_outgoing_link(int id)
		{
			// If first create the area and then fill in necessary info
			if (this->outgoing_links == nullptr)
			{
				this->outgoing_links = new int[1];
				this->outgoing_links[this->ports] = id;
				this->ports++;
			}

			/*
				If adding to an exisiting list: 
					Resize the array
					Add the id 
					Store the new number of ports to the page
			*/
			if(link_already_exist(id) == 0)
			{
				this->outgoing_links = (int*)realloc(this->outgoing_links, (this->ports + 1) * sizeof(int));
				this->outgoing_links[this->ports] = id;
				this->ports++;
			}
		}
};

void create_empty_size_five_array(Page** temp)
{
	Page* new_refrance = new Page(-1);
	
	for (int i = 0; i < 5; i++)
	{
		temp[i] = new_refrance;
	}
}

#endif