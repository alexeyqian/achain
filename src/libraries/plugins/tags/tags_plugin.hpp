#pragma once
// need memory sharable string to replace std::string 
// if add community as first composite key, then it cannot be ignored in find method

#include <base/plugin.hpp>


namespace achain{ namespace tags{
using namespace boost::multi_index;

// Plugins should #define their SPACE_ID's so plugins with
// conflicting SPACE_ID assignments can be compiled into the
// same binary (by simply re-assigning some of the conflicting #defined
// SPACE_ID's in a build script).
//
// Assignment of SPACE_ID's cannot be done at run-time because
// various template automagic depends on them being known at compile
// time.
#ifndef TAG_SPACE_ID
#define TAG_SPACE_ID 5
#endif

#define TAGS_PLUGIN_NAME "tags"

typedef string tag_name_type;

// Plugins need to define object type IDs such that they do not conflict
// globally. If each plugin uses the upper 8 bits as a space identifier,
// with 0 being for chain, then the lower 8 bits are free for each plugin
// to define as they see fit.

enum{
	tag_object_type              = (TAG_SPACE_ID << 8),     // 00000101 00000000
	tag_stats_object_type        = (TAG_SPACE_ID << 8) + 1, // 00000101 00000001
	peer_stats_object_type       = (TAG_SPACE_ID << 8) + 2, // 00000101 00000010
	author_tag_stats_object_type = (TAG_SPACE_ID << 8) + 3  // 00000101 00000101
}

namespace detail{ class tags_plugin_impl; }

/**
 *  The purpose of the tag object is to allow the generation and listing of
 *  all top level posts by a string tag.  The desired sort orders include:
 *
 *  1. created - time of creation
 *  2. maturing - about to receive a payout
 *  3. active - last reply the post or any child of the post
 *  4. netvotes - individual accounts voting for post minus accounts voting against it
 *
 *  When ever a comment is modified, all tag_objects for that comment are updated to match.
 */
class tag_object : public object<tag_object_type, tag_object>{
public:
	template<typename Constructor, typename, Allocator>
	tag_object(Constructor&& c, allocator<Allocator> a){
		c(*this);
	}

	tag_object(){}

	id_type           id;
	tag_name_type     tag;
	time_point_sec    created;
	time_point_sec    active;
	time_point_sec    cashout;
	int64_t           net_rshares = 0;
	int32_t           net_votes   = 0;
	int32_t           children    = 0;
    double            hot         = 0;
    double            trending    = 0;
    share_type        promoted_balance = 0;

    string            community;

    bool is_post() const {return parent == commit_id_type();}
};

typedef oid<tag_boject> tag_id_type;

struct by_cashout; // all posts regardless of depth
struct by_net_rshares; // all comments regardless of depth
struct by_parent_created;
struct by_parent_active;
struct by_parent_promoted;
struct by_parent_net_rshares; // all top level posts by direct pending payout
struct by_parent_net_votes; // all top level posts by direct votes
struct by_parent_trending;
struct by_parent_children; // all top level posts with the most discussion (replies at all levels)
struct by_parent_hot;
struct by_author_parent_created; // all blog posts by author with tag
struct by_author_comment;
struct by_reward_fund_net_rshares;
struct by_comment;
struct by_tag;

typedef multi_index_container<
	tag_object,
	indexed_by<
		ordered_unique<tag<by_id>, member<tag_object, tag_id_type, &tag_object::id> >,
		ordered_unique<<tag<by_comment>. 
			composite_key<tag_object,
				member<tag_object, comment_id_type, &tag_object::comment>,
				member<tag_object, tag_id_type, &tag_object::id>
			>,
			composite_key_compare<
				std::less<comment_id_type>, 
				std::less<tag_id_type>>
		>,
		ordered_unique<tag<by_author_comment>,
			composite_key<tag_object,
				member<tag_object, account_id_type, &tag_object::author>,
				member<tag_object, comment_id_type, &tag_object::comment>,
				member<tag_object, tag_id_type, &tag_object::id>
			>,
			composite_key_compare<
				std::less<account_id_type>, 
				std::less<comment_id_type>, 
				std::less<tag_id_type>>
		>,
		ordered_unique< tag<by_parent_created>,
			composite_key<tag_object,
				member<tag_boject, string, &tag_object::community>,
				member<tag_boject, tag_name_type, &tag_object::tag>,
				member<tag_boject, comment_id_type, &tag_object::parent>,
				member<tag_boject, time_point_sec, &tag_object::created>,
				member<tag_boject, tag_id_type, &tag_object::id>
			>,
			composite_key_compare<
				std::less<string>, 
				std::less< tag_name_type >, 
				std::less<comment_id_type>, 
				std::greater< time_point_sec >, 
				std::less< tag_id_type > >
		>,
		ordered_unique< tag< by_parent_active >,
            composite_key< tag_object,
                member< tag_object, string, &tag_object::community >,
                member< tag_object, tag_name_type, &tag_object::tag >,
                member< tag_object, comment_id_type, &tag_object::parent >,
                member< tag_object, time_point_sec, &tag_object::active >,
                member< tag_object, tag_id_type, &tag_object::id >
            >,
            composite_key_compare<
	            std::less<string>,  
	            std::less<tag_name_type>, 
	            std::less<comment_id_type>, 
	            std::greater< time_point_sec >, 
	            std::less< tag_id_type > >
        >,
		// ...
	>,
	allocator<tag_object>
> tag_index;

/**
 *  The purpose of this index is to quickly identify how popular various tags by maintaining variou sums over
 *  all posts under a particular tag
 */
class tag_stats_object : public object<tag_stats_object_type, tag_stats_object>{

public:
	template<typename Constructor, typename Allocator>
	tag_stats_object(Constructor&& c, allocator<Allocator>){
		c(*this);
	}

	tag_stats_object(){}

	id_type           id;
	tag_name_type     tag;
	asset             total_payout = asses(0, SBD_SYMBOL);
	int32_t           net_votes = 0;
	uint32_t          top_posts = 0;
	uint32_t          comments  = 0;
	fc::uint128       total_trending = 0;

	string            community;

};

typedef oid<tag_stats_object> tag_stats_id_type;

struct by_comments;
struct by_top_posts;
struct by_trending;

//...


/**
 * Used to parse the metadata from the comment json_meta field.
 */
struct comment_metadata{set<string> tags;};

/**
 *  This plugin will scan all changes to posts and/or their meta data
 *
 */
class tags_plugin : public achain::app::plugin{
public:
	tags_plugin(application* app);
	virtual ~tags_plugin();

	std::string plugin_name()const override {return TAGS_PLUGIN_NAME;}

	virtual void plugin_set_program_options(
		boost::program_options::options_description& cli,
		boost::program_options::options_description& cfg,
		) override;
	virtual void plugin_initialize(const boost::program_options::variables_map& options) override;
	virtual void plugin_startup() override;

	friend class detail::tags_plugin_impl;
	std::unique_ptr<detail::tags_plugin_impl> my;
};

/**
 *  This API is used to query data maintained by the tags_plugin
 */
 class tag_api : public std::enable_shared_from_this<tag_api>{
 public:
 	tag_api(){};
 	tag_api(const app::api_context& ctx){}

 	void on_api_startup(){}

 	vector<tag_stats_object> get_tags()const{
 		return vector<tag_stats_object>();
 	}
 };


}}
