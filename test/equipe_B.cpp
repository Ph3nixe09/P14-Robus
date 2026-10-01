// PID
previous_error := 0
integral := 0

loop:
	error := setpoint − measured_value
	integral := integral + error × dt
	derivative := (error − previous_error) / dt
	output := Kp × error + Ki × integral + Kd × derivative
	previous_error := error
	wait(dt)
	goto loop

// Matrice de position
void position () {
    int position_initiale = 2;
    // Si la position_actuelle 
}