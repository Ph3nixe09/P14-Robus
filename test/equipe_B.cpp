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

// Matrice de position initiale
int position () {
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
                        // horizontal_position += 2
                    // Non
                        // J'avance de 1 m.
                        // vertical_position += 1
            // Non
                // J'avance de 1,0 m.
                // vertical_position += 1
    return int position
}


// mettre des limites de position ex: peut pas aller en bas de 1 ou plus haut que 3
// éviter de changer la position trop vite lorsqu'il est au milieu pour éviter qu'il sorte des limites

// Matrice de position horizontale