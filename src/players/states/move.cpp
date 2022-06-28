#include "players/states/move.hpp"

Move::Move (std::string player_name, std::string state_name, MoveArea move_area, StateType state_type, float damage,
            float knockback_x, float knockback_y, int stun, bool is_knockdown, bool is_grab,
            std::function<void ()> update_function, int fps)
    : State (player_name, state_name, update_function, state_type, fps), move_area (move_area), damage (damage),
      knockback_x (knockback_x), knockback_y (knockback_y), stun (stun), is_knockdown (is_knockdown), is_grab (is_grab)
{
}
