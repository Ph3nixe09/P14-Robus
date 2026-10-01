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
    int initial_horizontal_position = 2;
    int initial_vertical_position = 0;
    // Si la horizontal_position est égale à 2:
        // Est-ce qu'il y a un objet devant moi?
            // Oui
                // Je me tourne vers la gauche et vérifie s'il y a un objet.
                // horizontal_position -= 1
                // Est-ce qu'il y a un objet devant moi?
                    // Oui
                        // Je me tourne vers la droite, j'avance de 1 m et j'avance
                        horizontal_position += 2
                    // Non
                        // J'avance de 1 m.s
            // Non
                // J'avance de 1,0 m.
                // vertical_position += 1
}